# How to generate a new WPC Qi 1.3 compliant certificate chain on Linux (RPi3)

## Requirements

Before you start please make sure you clone or download this repository. `git clone https://github.com/Infineon/wpcqi-optiga-trust-charge`

1. Install Python 3.7+; e.g. `apt-get install python3`
1. Install OpenSSL; e.g. `apt-get install libssl-dev`
2. Open a terminal
3. run `python --version` (output shall be: `Python 3.7` (or higher))
6. Install `optigatrust` python module,  `pip install optigatrust`
7. Connect the OPTIGA Trust Sample to you target system over an i2c. 
  **Note: If you have other than `armv7l` hardware architecture, or the OPTIGA is connected to other than /dev/i2c-1 interface you might need to port the python library for you platform. Follow [this](https://infineon.github.io/python-optiga-trust/linux_support.html) guide**
8. `python`
9. As a test try the following, check whether the output is similar to the one below
    ```bash
    C:\git\wpcqi-optiga-trust-charge\certificates\python-optiga-trust>python
    Python 3.8.1 (tags/v3.8.1:1b293b6, Dec 18 2019, 22:39:24) [MSC v.1916 32 bit (Intel)] on win32
    Type "help", "copyright", "credits" or "license" for more information.
    >>> from optigatrust import crypto
    >>> crypto.random(8)
    Loaded: liboptigatrust-libusb-win-i686.dll
    ================== OPTIGA Trust Chip Info ==================
    Firmware Identifier           [dwFirmwareIdentifier]:0x80101071
    Build Number                  [rgbESWBuild]:0x809
    Current Limitation            [OID: 0xE0C4]:0xf
    Sleep Activation Delay        [OID: 0xE0C3]:0x14
    Global Lifecycle State        [OID: 0xE0C0]:operational
    Security Status               [OID: 0xE0C1]:0x0
    Security Event Counter        [OID: 0xE0C5]:0x0
    ============================================================
    b'C\xd6TZ\x1crJ\xf8'
    >>> quit()
    ```
 1. Configure your setup by modifying the `new_certchain_conf.json` file.
    
   ```json
   {
        "openssl_path": "/usr/bin/openssl",
        "ptmc": "<your_ptmc_value>",
        "common_name": "<data_for_the_common_name_includes_wpc_qi_id_max35_symbols>",
        "rsid": "<revocation_sequential_number_max_18_symbols>",
        "slot": "<one_of_two_certificates_slots_to_populate>"
        "com_port": "<can_be_ignored>"
    }
   ```

 1. Certificate Chain generation
 
    ```bash
    C:\git\wpcqi-optiga-trust-charge\certificates>python new_certchain.py
    Loaded: liboptigatrust-libusb-win-i686.dll
    ================== OPTIGA Trust Chip Info ==================
    Firmware Identifier           [dwFirmwareIdentifier]:0x80101071
    Build Number                  [rgbESWBuild]:0x809
    Current Limitation            [OID: 0xE0C4]:0xf
    Sleep Activation Delay        [OID: 0xE0C3]:0x14
    Global Lifecycle State        [OID: 0xE0C0]:operational
    Security Status               [OID: 0xE0C1]:0x0
    Security Event Counter        [OID: 0xE0C5]:0x0
    ============================================================
    [{'type': 'extension_request', 'values': [[{'extn_id': 'wpc-qi-authRSID', 'critical': True, 'extn_value': <asn1crypto.core.OctetString 49869488 b'\x04\t\x00\x00\x00\x00\x00\x00\x00\x00\x01'>}]]}]
    read EC key
    writing EC key
            1 Datei(en) verschoben.
    read EC key
    writing EC key
    C:\Users\Yushev\git\wpcqi-optiga-trust-charge\certificates\new_certchain\serial.txt* konnte nicht gefunden werden
    C:\Users\Yushev\git\wpcqi-optiga-trust-charge\certificates\new_certchain\certindex.txt* konnte nicht gefunden werden
            1 Datei(en) kopiert.
            1 Datei(en) kopiert.
    Using configuration from IFX_WPC_QI_13_CA_Config.cfg
    Check that the request matches the signature
    Signature ok
    The Subject's Distinguished Name is as follows
    commonName            :ASN.1 12:'CAFE-1A'
    Certificate is to be certified until Dec 31 23:59:59 9999 GMT (2914246 days)
    
    Write out database with 1 new entries
    Certificate:
        Data:
            Version: 3 (0x2)
            Serial Number: 305441741 (0x1234abcd)
            Signature Algorithm: ecdsa-with-SHA256
            Issuer: CN=WPCCA1
            Validity
                Not Before: Jan  1 00:00:00 1970 GMT
                Not After : Dec 31 23:59:59 9999 GMT
            Subject: CN=CAFE-1A
            Subject Public Key Info:
                Public Key Algorithm: id-ecPublicKey
                    Public-Key: (256 bit)
                    pub:
                        02:f6:74:90:4f:c5:f2:d6:02:e4:0b:80:d0:a7:46:
                        b0:fb:be:5d:eb:e4:92:d4:a1:0f:2b:68:ff:21:68:
                        62:f8:53
                    ASN1 OID: prime256v1
                    NIST CURVE: P-256
            X509v3 extensions:
                X509v3 Basic Constraints: critical
                    CA:TRUE, pathlen:0
                2.23.255.1.1: critical
                    ......
        Signature Algorithm: ecdsa-with-SHA256
             30:46:02:21:00:80:57:85:75:4c:d1:34:a7:d1:41:eb:a2:61:
             cd:50:2a:3c:ef:ca:e2:d3:ba:bb:b8:ed:79:f5:50:02:1c:1d:
             41:02:21:00:f2:9f:1f:91:6f:9a:1d:1d:68:0e:7c:57:9d:00:
             40:06:d0:8e:11:87:a9:cc:07:ad:b1:3e:8c:eb:7d:31:e1:25
    -----BEGIN CERTIFICATE-----
    MIIBITCBx6ADAgECAgQSNKvNMAoGCCqGSM49BAMCMBExDzANBgNVBAMMBldQQ0NB
    MTAgFw03MDAxMDEwMDAwMDBaGA85OTk5MTIzMTIzNTk1OVowEjEQMA4GA1UEAwwH
    Q0FGRS0xQTA5MBMGByqGSM49AgEGCCqGSM49AwEHAyIAAvZ0kE/F8tYC5AuA0KdG
    sPu+XevkktShDyto/yFoYvhToyowKDASBgNVHRMBAf8ECDAGAQH/AgEAMBIGBWeB
    fwEBAQH/BAYEBAAAAAAwCgYIKoZIzj0EAwIDSQAwRgIhAIBXhXVM0TSn0UHromHN
    UCo878ri07q7uO159VACHB1BAiEA8p8fkW+aHR1oDnxXnQBABtCOEYepzAetsT6M
    630x4SU=
    -----END CERTIFICATE-----
    Data Base Updated
            1 Datei(en) verschoben.
            1 Datei(en) kopiert.
            1 Datei(en) kopiert.
    Using configuration from IFX_WPC_QI_13_CA_Config.cfg
    Check that the request matches the signature
    Signature ok
    The Subject's Distinguished Name is as follows
    commonName            :ASN.1 12:'006386-Model5'
    userId                :ASN.1 12:'SEPT-23'
    Certificate is to be certified until Dec 31 23:59:59 9999 GMT (2914246 days)
    
    Write out database with 1 new entries
    Certificate:
        Data:
            Version: 3 (0x2)
            Serial Number: 305458945 (0x1234ef01)
            Signature Algorithm: ecdsa-with-SHA256
            Issuer: CN=CAFE-1A
            Validity
                Not Before: Jan  1 00:00:00 1970 GMT
                Not After : Dec 31 23:59:59 9999 GMT
            Subject: CN=006386-Model5
            Subject Public Key Info:
                Public Key Algorithm: id-ecPublicKey
                    Public-Key: (256 bit)
                    pub:
                        04:0d:c9:63:63:65:d0:ca:a3:0c:35:a8:3d:f8:3d:
                        74:31:7f:1e:c1:c7:64:e5:a1:c5:f3:bb:6c:ab:e8:
                        1c:67:43:83:fe:5e:44:04:e6:d1:35:c7:96:39:3a:
                        a4:cf:09:78:f5:d9:c4:23:9b:89:91:60:25:b0:c6:
                        72:00:04:b1:78
                    ASN1 OID: prime256v1
                    NIST CURVE: P-256
            X509v3 extensions:
                2.23.255.1.2: critical
                    ...........
        Signature Algorithm: ecdsa-with-SHA256
             30:44:02:20:10:9e:63:b0:bb:c2:d4:8d:14:39:9b:e8:d3:74:
             b3:5b:0b:b1:34:1a:90:3d:25:ed:e3:de:e3:af:12:ab:1b:8a:
             02:20:67:31:38:d0:84:f8:6f:af:97:85:fe:47:7e:b4:da:66:
             63:7c:c7:90:63:2f:24:d4:bb:28:10:6e:c3:0d:b5:c4
    -----BEGIN CERTIFICATE-----
    MIIBNzCB36ADAgECAgQSNO8BMAoGCCqGSM49BAMCMBIxEDAOBgNVBAMMB0NBRkUt
    MUEwIBcNNzAwMTAxMDAwMDAwWhgPOTk5OTEyMzEyMzU5NTlaMBgxFjAUBgNVBAMM
    DTAwNjM4Ni1Nb2RlbDUwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQNyWNjZdDK
    oww1qD34PXQxfx7Bx2TlocXzu2yr6BxnQ4P+XkQE5tE1x5Y5OqTPCXj12cQjm4mR
    YCWwxnIABLF4oxswGTAXBgVngX8BAgEB/wQLBAkAAAAAAAAAAAEwCgYIKoZIzj0E
    AwIDRwAwRAIgEJ5jsLvC1I0UOZvo03SzWwuxNBqQPSXt497jrxKrG4oCIGcxONCE
    +G+vl4X+R3602mZjfMeQYy8k1LsoEG7DDbXE
    -----END CERTIFICATE-----
    Data Base Updated
            1 Datei(en) verschoben.
    
    C:\Users\Yushev\git\wpcqi-optiga-trust-charge\certificates>
    ```
1. For your reference, the resulting certificate chain is stored in DER format in `IFX_WPC_QI_13_Certificate_chain.bin`
1. Test Root CA, Test Manufacturer CA and Test Product Unit CA are stored in the folder `new_certchain`
1. To use this new certificate chain with the appnote, don't forget to update the Root CA Certificate you use in [the code](https://github.com/Infineon/wpcqi-optiga-trust-charge/blob/master/wpc/qi_auth_self_test.c#L114). Instead of `qi_auth_cr660_draft5_ifx_root_cert` use `qi_auth_test_new_certchain`  

## How does it work

* The script `new_certchain.py`
    * reads out the configuration file `new_certchain_conf.json`
    * initializes connected OPTIGA chip and generates a new keypair on the chip
    * read out from the chip the public key pair of newly generated keypair
    * constructs a Certificate Signing Request according to the configuration file. The result is stored in `wpc_qi_testchain.csr.pem`
    * using openssl calls constructs a new certificate chain based on existing Test Root CA
    * collects the resulting files and prepares one certificate chain
    * Resulting chain is store here `IFX_WPC_QI_13_Certificate_chain.bin`
