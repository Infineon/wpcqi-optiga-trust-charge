import subprocess
import os
import sys
import hashlib
import json
from asn1crypto import core as c

import optigatrust as optiga
from optigatrust import objects, crypto

from jinja2 import Environment, FileSystemLoader

import wpcqi_csr as csr

# Certificate and key oids which are used to create a new key. The user might affect this by defining the slot in config
certificate_slot = 0xe0e0
key_slot = 0xe0f0

# This isn't a parameter, if you would like to change this value, other parts of the script should be adopted
prefix = 'IFX_WPC_QI_13'

# Current working directory
work_dir = os.getcwd()

# Folder where everything generated is stored
certchain_dir = os.path.join(work_dir, 'new_certchain')
certchain_conf_dir = os.path.join(certchain_dir, '_static')


def calculate_root_ca_hash(root_ca_path: str):
    try:
        with open(root_ca_path, 'rb') as f:
            # Construct a hash object using our selected hashing algorithm
            h = hashlib.sha256()
            # Update the hash using a bytes object
            h.update(f.read())
            return h.digest()
    except IOError as e:
        print(e)


def write_cert_chain_os(root_ca_path: str, man_ca_cert_path: str, prod_unit_cert_path: str, rsid) -> bytes:
    # Calculate Hash of the certificate, it comes first in teh chain
    root_ca_hash = calculate_root_ca_hash(root_ca_path)

    # Calculate length of the certificate chain
    root_ca_size = len(root_ca_hash)
    man_ca_size = os.stat(man_ca_cert_path).st_size
    prod_unit_size = os.stat(prod_unit_cert_path).st_size
    chain_size_bytes = (root_ca_size + man_ca_size + prod_unit_size).to_bytes(2, 'big')

    # Consolidate all information in one file
    path_final_certchain = os.path.normpath(os.path.abspath('IFX_WPC_QI_13_Certificate_chain_{0}.bin'.format(rsid)))
    try:
        with open(man_ca_cert_path, 'rb') as man_ca_file:
            # Here we add our manufacturer ca certificate
            man_ca_cert = man_ca_file.read()
        with open(prod_unit_cert_path, 'rb') as prod_unit_file:
            # In the end the product unit certificate
            prod_unit_cert = prod_unit_file.read()
        with open(path_final_certchain, 'wb+') as chain_file:
            # First in the chain comes the c6 tag (for optiga), then 2 bytes length of the chain (for optiga),
            # then again 2 bytes length, then hash, then manufacturer ca certificate,
            # then product unit certificate
            bytes_to_write = b'\xc6' + chain_size_bytes + chain_size_bytes + root_ca_hash + man_ca_cert + prod_unit_cert
            chain_file.write(bytes_to_write)
            return bytes_to_write
    except IOError as e:
        print(e)
        exit()


def write_cert_chain_optiga(cert_chain: bytes, slot):
    cert_obj = optiga.Object(slot)

    # This will work only if the lcso is less than operational
    old_meta = cert_obj.meta['change']
    cert_obj.meta = {'change': 'always'}
    cert_obj.write(cert_chain)

    # bringing back the same value
    cert_obj.meta = {'change': old_meta}


def build_new_csr(slot, common_name, rsid):
    # check for input parameters
    csr_key = objects.ECCKey(slot)
    # unlock the key object, as the lcso for samples is in initialisation
    csr_key.meta = {'change': 'always', 'execute': 'always'}
    # generate a new key for the slot. it will become a base for the CSR
    public_key, _ = crypto.generate_pair(csr_key, curve='secp256r1')
    # lock back the key slot
    csr_key.meta = {'change': 'never'}

    # initialise the builder object to create the csr
    builder = csr.WPCQiBuilder(
        {
            'common_name': common_name,
        },
        public_key
    )

    # build the CSR with the required extension
    builder.set_extension('wpc-qi-authRSID', c.OctetString(bytes.fromhex(rsid)))

    request = builder.build_wpcqi(csr_key)

    # store the csr file locally
    path_csr = os.path.normpath(os.path.abspath(os.path.join(certchain_dir, 'wpc_qi_testchain.csr')))
    try:
        with open(path_csr, 'wb+') as f:
            f.write(request.dump())
    except IOError as e:
        print(e)
        exit()

    return path_csr, csr_key


def build_manufacturer_ca_conf(ptmc):
    name = 'IFX_WPC_QI_13_Manufacturing_Request'
    template_env = Environment(
        autoescape=False,
        loader=FileSystemLoader(certchain_conf_dir),
        trim_blocks=False)
    fname = "{0}.tmpl".format(name)

    context = {
        'ptmc': ptmc,
    }
    output = template_env.get_template(fname).render(context)
    path_to_template = os.path.normpath(os.path.abspath(os.path.join(certchain_conf_dir, "{0}.conf".format(name))))
    try:
        with open(path_to_template, "w+", encoding='utf8') as conf_file:
            conf_file.write(output)
    except IOError as e:
        print(e)
        exit()


def detect_correct_script_name():
    if sys.platform in ('linux', 'lunux1', 'linux2'):
        return 'testperso_wpc_qi.sh', True
    elif sys.platform == 'win32':
        return 'testperso_wpc_qi.cmd', False
    else:
        raise OSError('You target OS might be not supported')


def build_certificates(path_openssl, path_csr):
    script_name, linux = detect_correct_script_name()
    # we store current working directory to come back here afterwards
    old_wd = os.getcwd()
    os.chdir(certchain_conf_dir)
    if not os.path.exists(path_openssl):
        print('This path doesn\'t exist: {0}'.format(path_openssl))
    if not os.path.exists(path_csr):
        print('This path doesn\'t exist: {0}'.format(path_csr))
    if linux:
        subprocess.call('sh {0} {1} {2}'.format(script_name, path_openssl, path_csr), shell=True)
    else:
        subprocess.run([script_name, path_openssl, path_csr], shell=True)
    # come back to the directory
    os.chdir(old_wd)


def sanitize(field, max_size):
    field_name = [k for k, v in locals().items() if v == field][0]
    if len(field) > max_size:
        raise ValueError(
            '{0} field in the configuration file should be less than {1} characters, '
            'your length {2}'.format(field_name, max_size, len(field))
        )


def norm_path(core_path, file_prefix, file_name):
    return os.path.normpath( os.path.abspath( os.path.join(core_path, '{0}_{1}'.format(file_prefix, file_name))))


def main():
    global certificate_slot
    global key_slot

    # get path to openssl
    try:
        with open(os.path.abspath(os.getcwd() + '/new_certchain_conf.json'), 'r', encoding='utf-8') as f:
            data = json.load(f)
            path_openssl = data['openssl_path']
            ptmc = data['ptmc']
            common_name = data['common_name']
            rsid = data['rsid']
            slot = data['slot']
            com_port = data['com_port']
    except IOError as e:
        print(e)
        exit()

    try:
        sanitize(ptmc, 7)
        sanitize(common_name, 35)
        sanitize(rsid, 18)
        sanitize(slot, 1)
    except ValueError as e:
        print(e)
        exit()

    if com_port != '':
        optiga.set_com_port(com_port)

    # detect which certificate and key slots to use on optiga based on slot selection
    slot = int(slot)
    if slot != 0 and slot != 1:
        raise ValueError('slot should be either 0 or 1, you have {0}'.format(slot))

    # BAsed on the config we detect the correct OID for the cert/keypair to use
    certificate_slot += slot
    key_slot += slot

    path_csr, csr_key = build_new_csr(key_slot, common_name, rsid)
    build_manufacturer_ca_conf(ptmc)
    build_certificates(path_openssl, path_csr)

    # Detect paths of the DER encoded certificates produced by the script
    path_root_ca = norm_path(certchain_conf_dir, prefix, 'Root_Certificate.crt')
    path_man_ca = norm_path(certchain_dir, prefix, 'Manufacturing_Certificate.crt')
    path_prod_unit = norm_path(certchain_dir, prefix, 'ProductUnit_Certificate.crt')

    # Concatenate all certificates and store them locally
    if not os.path.exists(path_root_ca):
        print("This path doesn't exist: {0}".format(path_root_ca))
    if not os.path.exists(path_man_ca):
        print("This path doesn't exist: {0}".format(path_man_ca))
    if not os.path.exists(path_prod_unit):
        print("This path doesn't exist: {0}".format(path_prod_unit))
    cert_chain_bytes = write_cert_chain_os(path_root_ca, path_man_ca, path_prod_unit, rsid)

    # Store the resulting certificate chain on optiga
    write_cert_chain_optiga(cert_chain_bytes, certificate_slot)


if __name__ == "__main__":
    main()
