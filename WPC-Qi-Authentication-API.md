<a name="top"></a>

The WPC Qi Authentication API comprises two components
* [Qi Auth Crypt](../master/examples/qi_auth/qi_auth_crypt.c) to perform crypto related operations
* [Qi Auth](../master/examples/qi_auth/qi_auth.c) to prepare authentication packets according to WPC Qi Auth 1.3 specification

Almost all of the below mentioned functions are used in [this](../master/examples/qi_auth/qi_auth.c#L457) test routine

[tocstart]: # (toc start)

  * **Power Transmitter (PTx) API:**
    * qi_auth.h
      * [qi_auth_resp_error](#qi_auth_resp_error)
      * [qi_auth_resp_digests](#qi_auth_resp_digests)
      * [qi_auth_resp_certificate](#qi_auth_resp_certificate)
      * [qi_auth_resp_challenge_auth](#qi_auth_resp_challenge_auth)
    * qi_auth_crypt.h
      * [qi_auth_crypt_init](#qi_auth_crypt_init)
      * [qi_auth_crypt_certchain_sha256](#qi_auth_crypt_certchain_sha256)
      * [qi_auth_crypt_certchain](#qi_auth_crypt_certchain)
      * [qi_auth_crypt_generate_sha256](#qi_auth_crypt_generate_sha256)
      * [qi_auth_crypt_sign](#qi_auth_crypt_sign)
  * **Power Receiver PRx API** (WPC doesn't mandate the usage of the Secure Element on the receiver side):
    * qi_auth.h
      * [qi_auth_req_get_digests](#qi_auth_req_get_digests)
      * [qi_auth_req_get_certificate](#qi_auth_req_get_certificate)
      * [qi_auth_req_challenge](#qi_auth_req_challenge)
      * [qi_auth_req_handover](#qi_auth_req_handover)
      * [qi_auth_verify_chall_auth](#qi_auth_verify_chall_auth)
      * [qi_auth_verify_cert](#qi_auth_verify_cert)
    * qi_auth_crypt.h
      * [qi_auth_crypt_init](#qi_auth_crypt_init)
      * [qi_auth_crypt_get_random](#qi_auth_crypt_get_random)
      * [qi_auth_crypt_generate_sha256](#qi_auth_crypt_generate_sha256)
      * [qi_auth_crypt_verify_signature](#qi_auth_crypt_verify_signature)
      * [qi_auth_crypt_verify_certchain](#qi_auth_crypt_verify_certchain)
      * [qi_auth_crypt_parse_certchain](#qi_auth_crypt_parse_certchain)


[tocend]: # (toc end)


## qi_auth_resp_error

* **Prototype**
	```c
    uint16_t qi_auth_resp_error(uint8_t error_code, uint8_t error_data, uint8_t* p_resp, uint16_t* resp_size);
	```
* **Description:**  Returns an ERROR message in a form defined by WPC Qi Auth Rel 1.3
* **Arguments:**
	* `error_code`: Error code to be transmitted
	* `error_data`: Error data to be transmitted
	* `p_resp`: Output buffer
	* `resp_size`: Length of the buffer
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_req_get_digests

* **Prototype**
	```c
    uint16_t qi_auth_req_get_digests(uint8_t ptx, uint8_t slot, uint8_t* p_req, uint16_t* req_size);
	```
* **Description:**  Returns a GET DIGESTS message in a form defined by WPC Qi Auth Rel 1.3
* **Arguments:**
	* `ptx`: A value of 0 indicates that the Initiator is a PRx. A value of 1 indicates that the Initiator is a PTx.
	* `slot`: Slot number
	* `p_req`: Output buffer
	* `req_size`: Length of the buffer
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_resp_digests

* **Prototype**
	```c
    uint16_t qi_auth_resp_digests(uint8_t slot, uint8_t* p_resp, uint16_t* resp_size);
	```
* **Description:**  Returns a DIGEST message in a form defined by  WPC Qi Auth Rel 1.3
* **Arguments:**
	* `slot`: Slot number
	* `p_resp`: Output buffer where the digest message will be stored
	* `resp_size`: Length of the buffer
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_req_get_certificate

* **Prototype**
	```c
	uint16_t qi_auth_req_get_certificate(uint32_t offset, uint32_t length, uint8_t ptx, uint8_t slot,
                                         uint8_t* p_req, uint16_t* req_size);
	```
* **Description:**  Returns a GET_CERTIFICATE message in a form defined by WPC Qi Auth Rel 1.3
* **Arguments:**
	* `offset`: Offset.	These two fields combine to form the offset in bytes from
                the start of the Certificate Chain to where the read request begins offset value

	* `length`: Length in bytes to read. The length value is length98*256+length70. It is an error to specify a length that
                would result in reading beyond end of the Certificate Chain
	* `ptx`: A value of 0 indicates that the Initiator is a PRx. A value of 1 indicates that the Initiator is a PTx.
	* `slot`: Slot number
	* `p_req`: Output buffer
	* `req_size`: Length of the buffer
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_resp_certificate

* **Prototype**
	```c
    uint16_t qi_auth_resp_certificate(uint8_t slot, uint16_t offset, uint16_t length,
                                      uint8_t* p_resp, uint16_t* resp_size);
	```
* **Description:**  Returns a Certificate Chain in a form defined by WPC Qi Auth Rel 1.3
* **Arguments:**
	* `slot`: Slot number
	* `offset`: Offset
	* `length`: Length of the certificate chain
	* `p_resp`: Output buffer where the certificate message will be stored
	* `resp_size`: Length of the buffer
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_req_challenge

* **Prototype**
	```c
    uint16_t qi_auth_req_challenge(uint8_t ptx, uint8_t slot, uint8_t* p_creq, uint16_t* req_size);
	```
* **Description:**  Returns a Challenge message in a form defined by WPC Qi Auth Rel 1.3
* **Arguments:**
	* `ptx`: A value of 0 indicates that the Initiator is a PRx. A value of 1 indicates that the Initiator is a PTx.
	* `slot`: Slot number
	* `p_req`: Output buffer
	* `req_size`: Length of the buffer
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_resp_challenge_auth

* **Prototype**
	```c
    uint16_t qi_auth_resp_challenge_auth(uint8_t slot, uint8_t* p_challreq, uint8_t challreq_size,
		                                 uint8_t* p_resp, uint8_t* resp_size);
	```
* **Description:**  Returns a Challenge message in a form defined by WPC Qi Auth Rel 1.3
* **Arguments:**
	* `slot`: Slot number
	* `p_challreq`: Input Challenge request 
	* `challreq_size`: Length of the challenge
	* `p_resp`: Output buffer where the challenge auth message will be stored
	* `resp_size`: Length of the buffer
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_req_handover

* **Prototype**
	```c
    uint16_t qi_auth_req_handover(uint8_t* p_req, uint16_t* req_size);
	```
* **Description:**  Returns a HANDOVER mesage in a form defined by WPC Qi Auth Rel 1.3 
* **Arguments:**
	* `p_req`: Output buffer where the handover message will be stored
	* `req_size`: Length of the buffer
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_verify_chall_auth

* **Prototype**
	```c
    uint16_t qi_auth_verify_chall_auth(uint8_t* p_certchain, uint16_t certchain_size,
		                                  uint8_t* p_challreq, uint16_t challreq_size,
		                                  uint8_t* p_challresp, uint16_t challresp_size);
	```
* **Description:**  Verifies a given Response message in a form defined by WPC Qi Auth Rel 1.3. Challenge auth message is given as an input
* **Arguments:**
	* `p_challreq`: Input Challenge request 
	* `challreq_size`: Length of the challenge
	* `p_challresp`: Input Challenge response 
	* `challresp_size`: Length of the challenge response 
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_verify_chall_auth

* **Prototype**
	```c
    Verifies a given Certificate Chain message in a form defined by WPC Qi Auth Rel 1.3. Certificate Chain is given as an input
	```
* **Description:**  Verifies a given Response message in a form defined by WPC Qi Auth Rel 1.3. Challenge auth message is given as an input
* **Arguments:**
	* `p_certchain`: Certificate Chain message
	* `chain_size`: Chain size
* **Returns:** 
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_init

* **Prototype**
	```c
    int32_t qi_auth_crypt_init(void);
	```
* **Description:**  Initialises the PAL Crypt module
* **Returns:**
    * Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_certchain

* **Prototype**
	```c
    uint16_t qi_auth_crypt_certchain(uint8_t* p_certchain, uint16_t* p_certchain_size);
	```
* **Description:**  Return the certificate chain provisioned into the chip
* **Arguments:**
	* `p_certchain`: Pointer to the buffer where the certificate chain should be stored
	* `p_certchain_size`: SBuffer size, should be at least 277 bytes
* **Returns:** 
	* Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_sign

* **Prototype**
	```c
    uint16_t qi_auth_crypt_sign(uint8_t* p_digest_tbs, uint16_t digest_tbs_size, uint8_t* p_signature, uint16_t* p_signature_len);
	```
* **Description:**  Sign data with the chips private key
* **Arguments:**
	* `p_digest_tbs`: Pointer to the digest to be signed
	* `digest_tbs_size`: Buffer size (32 in general)
	* `p_signature`: Pointer to the buffer to store the signature
	* `p_signature_len`: Signature buffer size
* **Returns:** 
	* Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_certchain_sha256

* **Prototype**
	```c
    uint16_t qi_auth_crypt_certchain_sha256(uint8_t* p_digest);
	```
* **Description:**   Hash the certificate chain on chip and return its digest
* **Arguments:**
	* `p_digest`: Pointer to the buffer to store the digest, should be 32 bytes long
* **Returns:** 
	* Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_generate_sha256

* **Prototype**
	```c
    uint16_t qi_auth_crypt_certchain_sha256(uint8_t* p_digest);
	```
* **Description:**   Generates hash of the given data
* **Arguments:**
	* `p_input`: Pointer to the buffer to be hashed. 8 bytes minimum.
	* `inlen`: Buffer size
	* `p_digest`: Pointer to the buffer to store the digest, should be 32 bytes long
* **Returns:** 
	* Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_parse_certchain

* **Prototype**
	```c
    uint16_t  qi_auth_crypt_parse_certchain(uint8_t* p_chain, uint16_t chain_size, certificate_container_t *p_crt, uint8_t max_chain_size);
	```
* **Description:**   Parses raw WPC certificate into a custom defined certificate structure. <br>

					- The raw certificate must be in WPC defined format.<br>
					- The function does not allocate any memory.<br>
					- Stores reference address/location from raw certificate data into the custom defined certificate structure.<br>
					- The following details are parsed from the raw certificate : <br>
					    - Public Key
					    - Signature
					    - Certificate Data
* **Arguments:**
	* `p_chain`: Pointer to raw chain of certificates
	* `chain_size`: Size of the buffer with the chain
	* `p_crt`: Pointer to Structure which holds parsed certificate data
	* `max_chain_size`: Maximum size of the certificate chain
* **Returns:** 
	* Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_verify_certchain

* **Prototype**
	```c
    uint16_t qi_auth_crypt_verify_certchain(uint8_t* p_qi_auth_chain_buf, uint16_t qi_auth_chain_buf_size, uint8_t* p_pubkey, uint8_t pubkey_size);
	```
* **Description:**   Verifies the PKI domain of the certificate chain. Local Root CA certificate / hash is used to verify the chain
* **Arguments:**
	* `p_qi_auth_chain_buf`: Pointer to the buffer, where a certificate chain is stored
	* `qi_auth_chain_buf_size`: Buffer size
	* `p_pubkey`: Pointer to the public key which will be populated by the function
	* `pubkey_size`: Size of the public key buffer
* **Returns:** 
	* Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_verify_signature

* **Prototype**
	```c
    uint16_t  qi_auth_crypt_verify_signature(const signature_container_t* p_signature_c);
	```
* **Description:**   Verifies the signature using the given public key
* **Arguments:**
	* `p_signature_с`: Pointer to structure which holds data for signature verification
* **Returns:** 
	* Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)

## qi_auth_crypt_get_random

* **Prototype**
	```c
    uint16_t  qi_auth_crypt_get_random(uint16_t random_len, uint8_t *p_random, uint16_t random_size);
	```
* **Description:**   This function generates random data bytes of requested length.
* **Arguments:**
	* `random_len`: Number of random bytes requested (8 minimum)
	* `p_random`: Pointer to output buffer
	* `random_size`: Size of the random buffer
* **Returns:** 
	* Device error code. See [Device Error Codes](Device-Error-Codes) Wiki Page
	* `CRYPT_LIB_OK` On success function will return an `CRYPT_LIB_OK` code
	* `CRYPT_LIB_ERROR` Error code

[Top](#top)
