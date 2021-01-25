import subprocess
import os
import hashlib
import json
from asn1crypto import core as c

from optigatrust import core, cert, asymmetric

certificate_slot = 0xe0e0
key_slot = 0xe0f0


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


def write_cert_chain_os(root_ca_path: str, man_ca_cert_path: str, prod_unit_cert_path: str, chip_id) -> bytes:
    # Calculate Hash of the certificate, it comes first in teh chain
    root_ca_hash = calculate_root_ca_hash(root_ca_path)

    # Calculate length of the certificate chain
    root_ca_size = len(root_ca_hash)
    man_ca_size = os.stat(man_ca_cert_path).st_size
    prod_unit_size = os.stat(prod_unit_cert_path).st_size
    chain_size_bytes = (root_ca_size + man_ca_size + prod_unit_size).to_bytes(2, 'big')

    # Consolidate all information in one file
    path_final_certchain = os.path.abspath('IFX_WPC_QI_13_Certificate_chain_{0}.bin'.format(chip_id))
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


def write_cert_chain_optiga(cert_chain: bytes):
    global certificate_slot

    cert_obj = core.Object(certificate_slot)

    # This will work only if the lcso is less than operational
    old_meta = cert_obj.meta['change']
    cert_obj.meta = {'change': 'always'}
    cert_obj.write(cert_chain)

    # bringing back the same value
    cert_obj.meta = {'change': old_meta}


def sanitize(field, max_size):
    field_name = [k for k, v in locals().items() if v == field][0]
    if len(field) > max_size:
        raise ValueError(
            '{0} field in the configuration file should be less than {1} characters, '
            'your length {2}'.format(field_name, max_size, len(field))
        )


def main():
    global certificate_slot
    global key_slot

    # get path to openssl
    try:
        with open(os.path.abspath(os.getcwd() + '/new_certchain_conf.json'), 'r', encoding='utf-8') as f:
            data = json.load(f)
            path_openssl = data['openssl_path']
            user_id = data['user_id']
            common_name = data['common_name']
            rsid = data['rsid']
            slot = data['slot']
    except IOError as e:
        print(e)
        exit()

    try:
        sanitize(user_id, 32)
        sanitize(common_name, 35)
        sanitize(rsid, 18)
        sanitize(slot, 1)
    except ValueError as e:
        print(e)
        exit()

    # detect which certificate and key slots to use on optiga based on slot selection
    slot = int(slot)
    if slot != 0 and slot != 1:
        raise ValueError('slot should be either 0 or 1, you have {0}'.format(slot))

    # we select the target slots on optiga based the given slot value
    certificate_slot += slot
    key_slot += slot

    # check for input parameters
    csr_key = asymmetric.EccKey(key_slot)
    # unlock the key object, as the lcso for samples is in initialisation
    csr_key.meta = {'change': 'always', 'execute': 'always'}
    # generate a new key for the slot. it will become a base for the CSR
    csr_key.generate(curve='secp256r1')
    # lock back the key slot
    csr_key.meta = {'change': 'never'}
    # store the chip id
    uid = csr_key.optiga.settings.uid
    chip_id = hex(uid.batch_num).lstrip("0x") + hex(uid.x_coord).lstrip("0x") + hex(uid.y_coord).lstrip("0x")

    # initialise the builder object to create the csr
    builder = cert.Builder(
        {
            'user_id': user_id,
            'common_name': common_name,
        },
        csr_key
    )

    # build the CSR with the required extension
    builder.set_extension('wpc-qi-authRSID', c.OctetString(bytes.fromhex(rsid)))

    request = builder.build_wpcqi(csr_key)

    # store the csr file locally
    path_csr = os.path.abspath('./wpc_qi_testchain.csr')
    try:
        with open(path_csr, 'wb+') as f:
            f.write(request.dump())
    except IOError as e:
        print(e)
        exit()

    # Detecting our own path
    path = os.path.abspath(os.getcwd() + '/new_certchain_scripts/')
    # detecting the path to the batch file
    path_to_script = os.path.abspath(path + '/testperso_wpc_qi.cmd')
    # we store current working directory to come back here afterwards
    old_wd = os.getcwd()
    os.chdir(path)
    subprocess.call([path_to_script, path_openssl, path_csr])
    os.chdir(old_wd)

    # Detect paths of the DER encoded certificates produced by the script
    path_root_ca = os.path.abspath(path + '/IFX_WPC_QI_13_Root_Certificate.crt')
    path_man_ca = os.path.abspath(path + '/IFX_WPC_QI_13_Manufacturing_Certificate.crt')
    path_prod_unit = os.path.abspath(path + '/IFX_WPC_QI_13_ProductUnit_Certificate.crt')

    cert_chain_bytes = write_cert_chain_os(path_root_ca, path_man_ca, path_prod_unit, chip_id)
    write_cert_chain_optiga(cert_chain_bytes)


if __name__ == "__main__":
    main()







