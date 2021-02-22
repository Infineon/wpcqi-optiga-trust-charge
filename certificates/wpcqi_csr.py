# ============================================================================
# The MIT License
# 
# Copyright (c) 2015-2018 Will Bond <will@wbond.net>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# This is a modified version of the original csrbuilder python module
# A link ot the repository https://github.com/wbond/csrbuilder
# Modifications are done with respect to signature methods
# ============================================================================
import inspect
import re
import textwrap
import base64

from asn1crypto import x509, keys, csr, pem
from asn1crypto import core as c

from optigatrust import crypto, objects

int_types = (int,)
str_cls = str

__all__ = [
    'WPCQiBuilder',
    'pem_armor_csr'
]


def _writer(func):
    """
    Decorator for a custom writer, but a default reader
    """

    name = func.__name__
    return property(fget=lambda self: getattr(self, '_%s' % name), fset=func)


def pem_armor_csr(certification_request):
    """
    Encodes a CSR into PEM format

    :param certification_request: An asn1crypto.csr.CertificationRequest object of the CSR to armor. Typically this
    is obtained from Builder.build().

    :return:
        A byte string of the PEM-encoded CSR
    """

    if not isinstance(certification_request, csr.CertificationRequest):
        raise TypeError(_pretty_message(
            '''
            certification_request must be an instance of
            asn1crypto.csr.CertificationRequest, not %s
            ''',
            _type_name(certification_request)
        ))

    return pem.armor(
        'CERTIFICATE REQUEST',
        certification_request.dump()
    )


class WPCQiBuilder(object):
    _subject = None
    _subject_public_key = None
    _hash_algo = None
    _basic_constraints = None
    _subject_alt_name = None
    _key_usage = None
    _extended_key_usage = None
    _other_extensions = None

    _special_extensions = {'basic_constraints', 'subject_alt_name', 'key_usage', 'extended_key_usage'}

    def __init__(self, subject, subject_public_key):
        """
        Unless changed, CSRs will use SHA-256 for the signature
        :param subject:
            An asn1crypto.x509.Name object, or a dict - see the docstring
            for .subject for a list of valid options
        :param subject_public_key:
            An asn1crypto.keys.PublicKeyInfo object containing the public key
            the certificate is being requested for
        """

        self.subject = subject
        self.subject_public_key = subject_public_key
        self.ca = False

        self._hash_algo = 'sha256'
        self._other_extensions = {}
        # Here are two new extensions which we need for the library
        x509.ExtensionId._map['2.23.255.1.2'] = 'wpc-qi-authRSID'
        x509.Extension._oid_specs['wpc-qi-authRSID'] = c.OctetString

    @_writer
    def subject(self, value):
        """
        An asn1crypto.x509.Name object, or a dict with at least the
        following keys:
         - country_name
         - state_or_province_name
         - locality_name
         - organization_name
         - common_name
        Less common keys include:
         - organizational_unit_name
         - email_address
         - street_address
         - postal_code
         - business_category
         - incorporation_locality
         - incorporation_state_or_province
         - incorporation_country
        Uncommon keys include:
         - surname
         - title
         - serial_number
         - name
         - given_name
         - initials
         - generation_qualifier
         - dn_qualifier
         - pseudonym
         - domain_component
        All values should be unicode strings
        """

        is_dict = isinstance(value, dict)
        if not isinstance(value, x509.Name) and not is_dict:
            raise TypeError(_pretty_message(
                '''
                subject must be an instance of asn1crypto.x509.Name or a dict,
                not %s
                ''',
                _type_name(value)
            ))

        if is_dict:
            value = x509.Name.build(value)

        self._subject = value

    @_writer
    def subject_public_key(self, _value):
        self._subject_public_key = keys.PublicKeyInfo.load(_value)

    @_writer
    def hash_algo(self, value):
        """
        A unicode string of the hash algorithm to use when signing the
        request - "sha1" (not recommended), "sha256" or "sha512"
        """

        if value not in {'sha256', 'sha384'}:
            raise ValueError(_pretty_message(
                '''
                hash_algo must be one of "sha1", "sha256", "sha512", not %s
                ''',
                repr(value)
            ))

        self._hash_algo = value

    @property
    def ca(self):
        """
        None or a bool - if the request is for a CA cert. None indicates no
        basic constraints extension request.
        """

        if self._basic_constraints is None:
            return None

        return self._basic_constraints['ca'].native

    @ca.setter
    def ca(self, value):
        if value is None:
            self._basic_constraints = None
            return

        self._basic_constraints = x509.BasicConstraints({'ca': bool(value)})

        if value:
            self._key_usage = x509.KeyUsage({'key_cert_sign', 'crl_sign'})
            self._extended_key_usage = x509.ExtKeyUsageSyntax(['ocsp_signing'])
        else:
            self._key_usage = x509.KeyUsage({'digital_signature', 'key_encipherment'})
            self._extended_key_usage = x509.ExtKeyUsageSyntax(['server_auth', 'client_auth'])

    @property
    def subject_alt_domains(self):
        """
        A list of unicode strings of all domains in the subject alt name
        extension request. Empty list indicates no subject alt name extension
        request.
        """

        return self._get_subject_alt('dns_name')

    @subject_alt_domains.setter
    def subject_alt_domains(self, value):
        self._set_subject_alt('dns_name', value)

    @property
    def subject_alt_ips(self):
        """
        A list of unicode strings of all IPs in the subject alt name extension
        request. Empty list indicates no subject alt name extension request.
        """

        return self._get_subject_alt('ip_address')

    @subject_alt_ips.setter
    def subject_alt_ips(self, value):
        self._set_subject_alt('ip_address', value)

    def _get_subject_alt(self, name):
        """
        Returns the native value for each value in the subject alt name
        extension reqiest that is an asn1crypto.x509.GeneralName of the type
        specified by the name param
        :param name:
            A unicode string use to filter the x509.GeneralName objects by -
            is the choice name x509.GeneralName
        :return:
            A list of unicode strings. Empty list indicates no subject alt
            name extension request.
        """

        if self._subject_alt_name is None:
            return []

        output = []
        for general_name in self._subject_alt_name:
            if general_name.name == name:
                output.append(general_name.native)
        return output

    def _set_subject_alt(self, name, values):
        """
        Replaces all existing asn1crypto.x509.GeneralName objects of the choice
        represented by the name parameter with the values
        :param name:
            A unicode string of the choice name of the x509.GeneralName object
        :param values:
            A list of unicode strings to use as the values for the new
            x509.GeneralName objects
        """

        if self._subject_alt_name is not None:
            filtered_general_names = []
            for general_name in self._subject_alt_name:
                if general_name.name != name:
                    filtered_general_names.append(general_name)
            self._subject_alt_name = x509.GeneralNames(filtered_general_names)
        else:
            self._subject_alt_name = x509.GeneralNames()

        if values is not None:
            for value in values:
                new_general_name = x509.GeneralName(name=name, value=value)
                self._subject_alt_name.append(new_general_name)

        if len(self._subject_alt_name) == 0:
            self._subject_alt_name = None

    @property
    def key_usage(self):
        """
        A set of unicode strings representing the allowed usage of the key.
        Empty set indicates no key usage extension request.
        """

        if self._key_usage is None:
            return set()

        return self._key_usage.native

    @key_usage.setter
    def key_usage(self, value):
        if not isinstance(value, set) and value is not None:
            raise TypeError(_pretty_message(
                '''
                key_usage must be an instance of set, not %s
                ''',
                _type_name(value)
            ))

        if value == set() or value is None:
            self._key_usage = None
        else:
            self._key_usage = x509.KeyUsage(value)

    @property
    def extended_key_usage(self):
        """
        A set of unicode strings representing the allowed usage of the key from
        the extended key usage extension. Empty set indicates no extended key
        usage extension request.
        """

        if self._extended_key_usage is None:
            return set()

        return set(self._extended_key_usage.native)

    @extended_key_usage.setter
    def extended_key_usage(self, value):
        if not isinstance(value, set) and value is not None:
            raise TypeError(_pretty_message(
                '''
                extended_key_usage must be an instance of set, not %s
                ''',
                _type_name(value)
            ))

        if value == set() or value is None:
            self._extended_key_usage = None
        else:
            self._extended_key_usage = x509.ExtKeyUsageSyntax(list(value))

    def set_extension(self, name, value):
        """
        Sets the value for an extension using a fully constructed Asn1Value object from asn1crypto. Normally this should
        not be needed, and the convenience attributes should be sufficient. See the definition of
        asn1crypto.x509.Extension to determine the appropriate object type for a given extension. Extensions are marked
        as critical when RFC5280 or RFC6960 indicate so. If an extension is validly marked as critical or not
        (such as certificate policies and extended key usage), this class will mark it as non-critical.

        :param name: A unicode string of an extension id name from asn1crypto.x509.ExtensionId

        :param value:
            A value object per the specs defined by asn1crypto.x509.Extension
        """

        extension = x509.Extension({
            'extn_id': name
        })
        # We use native here to convert OIDs to meaningful names
        name = extension['extn_id'].native

        spec = extension.spec('extn_value')

        if not isinstance(value, spec) and value is not None:
            raise TypeError(_pretty_message(
                '''
                value must be an instance of %s, not %s
                ''',
                _type_name(spec),
                _type_name(value)
            ))

        if name in self._special_extensions:
            setattr(self, '_%s' % name, value)
        else:
            if value is None:
                if name in self._other_extensions:
                    del self._other_extensions[name]
            else:
                self._other_extensions[name] = value

    def _determine_critical(self, name):
        """
        :returns:
            A boolean indicating the correct value of the critical flag for
            an extension, based on information from RFC5280 and RFC 6960. The
            correct value is based on the terminology SHOULD or MUST.
        """

        if name == 'subject_alt_name':
            return len(self._subject) == 0

        if name == 'basic_constraints':
            return self.ca is True

        return {
            'subject_directory_attributes': False,
            'key_usage': True,
            'issuer_alt_name': False,
            'name_constraints': True,
            # Based on example EV certificates, non-CA certs have this marked
            # as non-critical, most likely because existing browsers don't
            # seem to support policies or name constraints
            'certificate_policies': False,
            'policy_mappings': True,
            'policy_constraints': True,
            'extended_key_usage': False,
            'inhibit_any_policy': True,
            'subject_information_access': False,
            'tls_feature': False,
            'ocsp_no_check': False,
        }.get(name, False)

    def build_wpcqi(self, signing_key):
        """
        Validates the certificate information, constructs an WPC Qi 1.3 X.509 certificate and then signs it
        :param signing_key: An asn1crypto.keys.PrivateKeyInfo or oscrypto.asymmetric.PrivateKey object for the private
        key to sign the request with. This should be the private key that matches the public key.

        :returns:
            An asn1crypto.csr.CertificationRequest object of the request
        """

        if not isinstance(signing_key, objects.ECCKey):
            raise TypeError(_pretty_message(
                '''
                signing_private_key must be an instance of
                optigatrust.pk.ECCKey, not %s
                ''',
                _type_name(signing_key)
            ))
        else:
            signature_algo = 'ecdsa'

        signature_algorithm_id = '%s_%s' % (self._hash_algo, signature_algo)

        extensions = []
        for name in sorted(self._other_extensions.keys()):
            extensions.append({
                'extn_id': name,
                'critical': True,
                'extn_value': self._other_extensions[name]
            })

        attributes = []
        if extensions:
            attributes.append({
                'type': 'extension_request',
                'values': [extensions]
            })

        certification_request_info = csr.CertificationRequestInfo({
            'version': 'v1',
            'subject': self._subject,
            'subject_pk_info': self._subject_public_key,
            'attributes': attributes
        })

        s = crypto.ecdsa_sign(signing_key, certification_request_info.dump())

        return csr.CertificationRequest({
            'certification_request_info': certification_request_info,
            'signature_algorithm': {
                'algorithm': signature_algorithm_id,
            },
            'signature': s.signature
        })


def _pretty_message(string, *params):
    """
    Takes a multi-line string and does the following:
     - dedents
     - converts newlines with text before and after into a single line
     - strips leading and trailing whitespace
    :param string:
        The string to format
    :param *params:
        Params to interpolate into the string
    :returns:
        The formatted string
    """
    output = textwrap.dedent(string)

    # Unwrap lines, taking into account bulleted lists, ordered lists and
    # underlines consisting of = signs
    if output.find('\n') != -1:
        output = re.sub('(?<=\\S)\n(?=[^ \n\t\\d\\*\\-=])', ' ', output)

    if params:
        output = output % params

    output = output.strip()

    return output


def _type_name(value):
    """
    :param value:
        A value to get the object name of
    :returns:
        A unicode string of the object name
    """

    if inspect.isclass(value):
        cls = value
    else:
        cls = value.__class__
    if cls.__module__ in {'builtins', '__builtin__'}:
        return cls.__name__
    return '%s.%s' % (cls.__module__, cls.__name__)


def _append_length(data, last=False):
    data_with_length = bytearray(3)
    left = len(data)

    data_with_length[2] = left % 0x100

    left = left >> 8
    data_with_length[1] = left % 0x100

    if last:
        data_with_length[0] = 0xC0
    else:
        left = left >> 8
        data_with_length[0] = left % 0x100

    data_with_length.extend(data)

    return data_with_length


def _strip_cert(cert):
    if cert.split('\n')[0] != "-----BEGIN CERTIFICATE-----":
        raise ValueError(
            'Incorrect Certificate '
            'Should start with "-----BEGIN CERTIFICATE-----" your starts with {0}'.format(cert.split('\n')[0])
        )
    raw_cert = cert.replace('-----BEGIN CERTIFICATE-----', '')
    raw_cert = raw_cert.replace('-----END CERTIFICATE-----', '')
    raw_cert = raw_cert.replace("\n", "")
    der_cert = base64.b64decode(raw_cert)

    return der_cert


def _break_apart(f, sep, step):
    return sep.join(f[n:n + step] for n in range(0, len(f), step))


access_conditions = {
    'always': 0x00,
    'never': 0xff,
}