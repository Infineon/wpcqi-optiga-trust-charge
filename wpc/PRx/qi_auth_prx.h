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
* \file    qi_auth_prx.h
*
* \brief   This file defines APIs, types and data structures for WPC Qi Auth Rel 1.3 implementation.
*
* @{
*/
#ifndef _QI_AUTH_PRX_H_
#define _QI_AUTH_PRX_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#define PRX_OK                              0x00
#define PRX_ERROR                           0x01

#define PRX_BAD_SLOT                        0x11
#define PRX_BAD_SIZE                        0x12
#define PRX_BAD_BUFFER                      0x13
#define PRX_BAD_LENGTH                      0x14

/**
 * \brief Initialises the PAL Crypt module
 */
int32_t qi_auth_prx_init(void);

/**
 * \brief De-initialises the PAL Crypt module
 */
int32_t qi_auth_prx_deinit(void);

/**
* \brief Returns a GET DIGESTS message in a form defined by WPC Qi Auth Rel 1.3
* \param slot[in]         Slot number
* \param p_req[out]       Output buffer
* \param req_size[in][out]    Length of the buffer
*
* \retval Error Code
* \retval #PRX_OK
*/
uint16_t qi_auth_prx_get_digests(uint8_t slot, uint8_t* p_req, uint16_t* req_size);


/**
* \brief Returns a GET_CERTIFICATE message in a form defined by WPC Qi Auth Rel 1.3
* \param offset[in]       Offset.	These two fields combine to form the offset in bytes from
*                         the start of the Certificate Chain to where the read request begins offset value
* \param length[in]       Length in bytes to read. The length value is length98*256+length70. It is an error to specify a length that
*                         would result in reading beyond end of the Certificate Chain
* \param slot[in]	      Slot number
* \param p_req[out]       Output buffer
* \param req_size[in][out]    Length of the buffer
*
* \retval Error Code
* \retval #PRX_OK
*/
uint16_t qi_auth_prx_get_certificate(uint32_t offset, uint32_t length, uint8_t slot,
		                                    uint8_t* p_req, uint16_t* req_size);

/**
* \brief Returns a Challenge message in a form defined by WPC Qi Auth Rel 1.3
* \param slot[in]         Slot number
* \param p_req[out]       Output buffer
* \param req_size[in][out]    Length of the buffer
*
* \retval Error Code
* \retval #PRX_OK
*/
uint16_t qi_auth_prx_challenge(uint8_t slot, uint8_t* p_creq, uint16_t* req_size);


/**
* \brief Verifies a given Response message in a form defined by WPC Qi Auth Rel 1.3. Challenge auth message is given as an input
* \param p_sha256[in]       Pointer to the digest value over the complete chain
* \param p_puc_pubkey[in]   Pointer to the public key
* \param pubkey_size[in]    Public key length in bytes
* \param p_challreq[in]     Challenge request message
* \param challreq_size[in]  Challenge request message size
* \param p_challresp[in]    Challenge response message
* \param challresp_size[in] Challenge response message size
*
* \retval Error Code
* \retval #PRX_OK
*/
uint16_t qi_auth_prx_verify_chall_auth(uint8_t* p_sha256,
                                       uint8_t* p_puc_pubkey, uint16_t pubkey_size,
		                               uint8_t* p_challreq, uint16_t challreq_size,
		                               uint8_t* p_challresp, uint16_t challresp_size);

/**
* \brief Verifies a given Certificate Chain message in a form defined by WPC Qi Auth Rel 1.3. Certificate Chain is given as an input
* \param p_certchain[in]  Certificate Chain message
* \param chain_size[in]   Chain size
* \param p_root_ca[in]    Pointer to the Root CA Certificate which should be used for the chain verification (Should be DER encoded hex string)
* \param root_ca_size[in] Size of the Root CA Certificate
*
* \retval Error Code
* \retval #PRX_OK
*/
uint16_t qi_auth_prx_verify_cert(uint8_t* p_certchain, uint16_t chain_size, const uint8_t* p_root_ca, uint16_t root_ca_size);


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
* \retval #PRX_OK
*/
uint16_t qi_auth_prx_get_certchain_info(uint8_t* p_certchain, uint16_t chain_size,
		                                uint8_t* p_puc_rsid, uint16_t* p_puc_rsid_size,
									    uint8_t* p_sha256,
									    uint8_t* p_puc_pubkey, uint16_t* p_puc_pubkey_size);


#endif
