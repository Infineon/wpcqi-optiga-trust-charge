/**
* MIT License
*
* Copyright (c) 2020 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
*
* \file qi_auth_prx_crypt.h
*
* \brief   This file defines APIs, types and data structures for Crypto Wrapper.
* @{
*/
#ifndef _QI_AUTH_PRX_CRYPT_H_
#define _QI_AUTH_PRX_CRYPT_H_

#include <stdint.h>
#include "mbedtls/x509_crt.h"


///Generic error condition
#define CRYPT_LIB_ERROR                             0x0001
#define CRYPT_LIB_ERROR_BAD_PARAMETERS              0x0002
#define CRYPT_LIB_ERROR_HASH_FAILED                 0x0003
#define CRYPT_LIB_ERROR_ROOT_HASH_MISMATCH          0x0004
#define CRYPT_LIB_ERROR_PARSE_ROOT_CERT             0x0005
#define CRYPT_LIB_ERROR_PARSE_MANUF_CERT            0x0006
#define CRYPT_LIB_ERROR_VERIFY_MANUF_CERT           0x0007
#define CRYPT_LIB_ERROR_PARSE_PRODUCT_CERT          0x0008
#define CRYPT_LIB_ERROR_VERIFY_PRODUCT_CERT         0x0009
#define CRYPT_LIB_ERROR_MANUF_NOTCA                 0x000a
#define CRYPT_LIB_ERROR_PRODUCT_CERT_IS_REVOKED     0x000b
#define CRYPT_LIB_ERROR_EXTRACT_PKEY                0x000c

/// Library API execution is successful
#define CRYPT_LIB_OK                                0x0000

#define WPCQI_AUTH_CRT_ROOTHASH_LEN                 0x20
/****************************************************************************
 *
 * Common data structure used across all functions.
 *
 ****************************************************************************/

/**
 * \brief Structure which holds data for signature verification
 */
typedef struct signature_container
{
    ///Public Key
    uint8_t* p_public_key;
    uint8_t  public_key_size;

    /// Signature
    uint8_t* p_signature;
    uint8_t  signature_size;

    /// Message
    uint8_t* p_message;
    uint16_t message_size;
}signature_container_t;


/****************************************************************************
 *
 * Definitions for Crypto Libray APIs.
 *
 ****************************************************************************/
/**
 * \brief Initialises the PAL Crypt module
 */
int32_t qi_auth_prx_crypt_init(uint8_t restore_from_hibernate);


/**
 * \brief Deinitialise the Crypt module
 */
int32_t qi_auth_prx_crypt_deinit(uint8_t hibernate_chip);

/**
*
* Return the certificate chain provisioned into the chip <br>
*
* \param[out]       p_certchain         Pointer to the buffer where the certificate chain should be stored
* \param[out]        p_certchain_size    Should be at least 277 bytes
*
*\retval  Device Error
*\retval  #CRYPT_LIB_ERROR
*\retval  #CRYPT_LIB_OK
*
*/
uint16_t qi_auth_prx_crypt_certchain(uint8_t* p_certchain, uint16_t* p_certchain_size);

/**
*
* Sign data with the chips private key <br>
*
* \param[in]        p_digest_tbs     Pointer to the digest to be signed
* \param[in]        digest_tbs_size     Buffer size (32 in general)
* \param[out]       p_signature      Pointer to the buffer to store the signature
* \param[in, out]    p_signature_len  Signature buffer size
*
*\retval  Device Error
*\retval  #CRYPT_LIB_ERROR
*\retval  #CRYPT_LIB_OK
*
*/
uint16_t qi_auth_prx_crypt_sign(uint8_t* p_digest_tbs, uint16_t digest_tbs_size, uint8_t* p_signature, uint16_t* p_signature_len);

/**
*
* Hash the certificate chain on chip and return its digest<br>
*
* \param[out]       p_digest        Pointer to the buffer to store the digest, should be 32 bytes long
*
*\retval  Device Error
*\retval  #CRYPT_LIB_ERROR
*\retval  #CRYPT_LIB_OK
*
*/
uint16_t qi_auth_prx_crypt_certchain_sha256(uint8_t* p_digest);

/**
*
* Generates hash of the given data<br>
*
* \param[in]        p_input         Pointer to the buffer to be hashed
* \param[in]        inlen            Buffer size
* \param[out]       p_digest        Pointer to the buffer to store the digest
*
*\retval  Device Error
*\retval  #CRYPT_LIB_ERROR
*\retval  #CRYPT_LIB_OK
*
*/
uint16_t qi_auth_prx_crypt_generate_sha256(const uint8_t* p_input, uint16_t inlen, uint8_t* p_digest);

/**
*
* Verifies the PKI domain of the certificate chain.<br>
*
* \param p_qi_auth_chain_buf[in]             Pointer to the buffer, where a certificate chain is stored
* \param qi_auth_chain_buf_size[][in]        Buffer size
* \param p_root_ca[in]                       Pointer to the Root CA Certificate which should be used for the chain verification (Should be DER encoded hex string)
* \param root_ca_size[in]                    Size of the Root CA Certificate
*
*\retval  Device Error
*\retval  #CRYPT_LIB_ERROR
*\retval  #CRYPT_LIB_OK
*
*/
uint16_t qi_auth_prx_crypt_verify_certchain(uint8_t* p_qi_auth_chain_buf, uint16_t qi_auth_chain_buf_size, const uint8_t* p_root_ca, uint16_t root_ca_size);

/**
* \brief Extract some important information from the certificate chain and allows the host side to cache only fraction of data
* \param p_certchain[in]        Certificate Chain message
* \param chain_size[in]         Chain size
* \param rsid[out]              RSID value of the Product Unit Certificate
* \param p_rsid_size[in][out]   Size of the buffer with the RSID. SHould fit from 1 to 9 bytes
* \param p_sha256[in][out]      Digest of the complete chain. The buffer should fit exactly 32 bytes.
* \param p_puc_pubkey[out]      Public Key of the Product Unit Certificate
* \param p_puc_pubkey_size[in][out] Size of the public key in the Product Unit Certificate.
*                               Should fit PubKeyLength(uncopressed 64) + Compression Byte[1]
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_prx_crypt_get_certchain_info(uint8_t* p_certchain, uint16_t chain_size,
 		                                uint8_t* p_puc_rsid, uint16_t* p_puc_rsid_size,
 									    uint8_t* p_sha256,
 									    uint8_t* p_puc_pubkey, uint16_t* p_puc_pubkey_size);

/**
* Verifies the signature using the given public key.<br>
*
*
*\param[in] p_signature_с Pointer to structure which holds data for signature verification
*
*\retval  Device Error
*\retval  #CRYPT_LIB_ERROR
*\retval  #CRYPT_LIB_OK
*\retval  #CRYPT_LIB_VERIFY_SIGN_FAIL
*\retval  #CRYPT_LIB_LENZERO_ERROR
*/
uint16_t  qi_auth_prx_crypt_verify_signature(const signature_container_t* p_signature_c);

/**
* This function generates random data bytes of requested length. <br>
*
*\param[in] random_len        Number of random bytes requested
*\param[in,out] p_random      Pointer to output buffer
*\param[in]     random_size   Size of the random buffer
*
*\retval  Device Error
*\retval  #CRYPT_LIB_ERROR
*\retval  #CRYPT_LIB_OK
*/
uint16_t  qi_auth_prx_crypt_get_random(uint16_t random_len, uint8_t *p_random, uint16_t random_size);

#endif //_QI_AUTH_PRX_CRYPT_H_
