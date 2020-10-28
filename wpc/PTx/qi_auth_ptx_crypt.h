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
* \file qi_auth_ptx_crypt.h
*
* \brief   This file defines APIs, types and data structures for Crypto Wrapper.
* @{
*/
#ifndef _QI_AUTH_CRYPT_H_
#define _QI_AUTH_CRYPT_H_

#include <stdint.h>


///Generic error condition
#define CRYPT_LIB_ERROR                             0x0001

/// Library API execution is successful
#define CRYPT_LIB_OK                                0x0000

///Signature Verification failure
#define CRYPT_LIB_VERIFY_SIGN_FAIL                  (CRYPT_LIB_ERROR + 2)

///Length of input is zero
#define CRYPT_LIB_LENZERO_ERROR                     (CRYPT_LIB_ERROR + 3)



#define QI_AUTH_CRT_ROOTHASH_LEN                     0x20

#define QI_AUTH_CRT_VER_LEN                          0x01
#define QI_AUTH_CRT_TYPE_LEN                         0x01
    #define QI_AUTH_CRT_TYPE_ROOT                    0x01
    #define QI_AUTH_CRT_TYPE_INTERM                  0x02
    #define QI_AUTH_CRT_TYPE_LEAF                    0x03
#define QI_AUTH_CRT_SIGNOFF_LEN                      0x01
#define QI_AUTH_CRT_SERNUM_LEN                       0x09
#define QI_AUTH_CRT_ISSUERID_LEN                     0x06
#define QI_AUTH_CRT_SUBJID_LEN                       0x06
#define QI_AUTH_CRT_PUBKEY_LEN                       0x21
#define QI_AUTH_CRT_SIGN_LEN                         0x40

#define QI_AUTH_CRT_BASE                             0x00
#define QI_AUTH_CRT_VER_OFF                          QI_AUTH_CRT_BASE
#define QI_AUTH_CRT_TYPE_OFF                         QI_AUTH_CRT_VER_OFF + QI_AUTH_CRT_VER_LEN
#define QI_AUTH_CRT_SIGNOFF_OFF                      QI_AUTH_CRT_TYPE_OFF + QI_AUTH_CRT_TYPE_LEN
#define QI_AUTH_CRT_SERNUM_OFF                       QI_AUTH_CRT_SIGNOFF_OFF + QI_AUTH_CRT_SIGNOFF_LEN
#define QI_AUTH_CRT_ISSUERID_OFF                     QI_AUTH_CRT_SERNUM_OFF + QI_AUTH_CRT_SERNUM_LEN
#define QI_AUTH_CRT_SUBJID_OFF                       QI_AUTH_CRT_ISSUERID_OFF + QI_AUTH_CRT_ISSUERID_LEN
#define QI_AUTH_CRT_PUBKEY_OFF                       QI_AUTH_CRT_SUBJID_OFF + QI_AUTH_CRT_SUBJID_LEN
#define QI_AUTH_CRT_SIGN_OFF                         QI_AUTH_CRT_PUBKEY_OFF + QI_AUTH_CRT_PUBKEY_LEN

#define QI_AUTH_CRT_LEN                              QI_AUTH_CRT_SIGN_OFF + QI_AUTH_CRT_SIGN_LEN
#define QI_AUTH_MINCHAIN                             0x03
#define QI_AUTH_MINCHAIN_LEN                         QI_AUTH_CRT_ROOTHASH_LEN + QI_AUTH_CRT_LEN + QI_AUTH_CRT_LEN
/****************************************************************************
 *
 * Common data structure used across all functions.
 *
 ****************************************************************************/


/****************************************************************************
 *
 * Definitions for Crypto Libray APIs.
 *
 ****************************************************************************/
/**
 * \brief Initialises the PAL Crypt module
 */
int32_t qi_auth_ptx_crypt_init(uint8_t restore_from_hibernate);

/**
 * \brief De-initialises the PAL Crypt module
 */
int32_t qi_auth_ptx_crypt_deinit(uint8_t hibernate_chip);

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
uint16_t qi_auth_ptx_crypt_certchain(uint8_t* p_certchain, uint16_t* p_certchain_size);

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
uint16_t qi_auth_ptx_crypt_sign(uint8_t* p_digest_tbs, uint16_t digest_tbs_size, uint8_t* p_signature, uint16_t* p_signature_len);

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
uint16_t qi_auth_ptx_crypt_certchain_sha256(uint8_t* p_digest);

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
uint16_t qi_auth_ptx_crypt_generate_sha256(const uint8_t* p_input, uint16_t inlen, uint8_t* p_digest);

#endif //_QI_AUTH_CRYPT_H_
