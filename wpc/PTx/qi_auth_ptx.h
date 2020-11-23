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
* \file    qi_auth_ptx.h
*
* \brief   This file defines APIs, types and data structures for WPC Qi Auth Rel 1.3 implementation.
*
* @{
*/
#ifndef _QI_AUTH_PTX_H_
#define _QI_AUTH_PTX_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#define PTX_OK                                  0x00
#define PTX_ERROR                               0x01

#define PTX_BAD_SLOT                            0x21
#define PTX_BAD_SIZE                            0x22
#define PTX_BAD_BUFFER                          0x23
#define PTX_BAD_LENGTH                          0x24
#define PTX_CRYPT_HASH_FAILED                   0x25
#define PTX_CRYPT_CERT_FAILED                   0x26
#define PTX_CRYPT_SIGN_FAILED                   0x27

#define QI_AUTH_ERROR_INVALID_REQUEST			0x01
#define QI_AUTH_ERROR_UNSUPPORTED_PROTOCOL		0x02
#define QI_AUTH_ERROR_BUSY						0x03
#define QI_AUTH_ERROR_UNSPECIFIED				0x04
#define QI_AUTH_ERROR_COMMUNCIATION_ERROR		0x01

/**
 * \brief Initialises the PAL Crypt module
 */
int32_t qi_auth_ptx_init(void);

/**
 * \brief De-initialises the PAL Crypt module
 */
int32_t qi_auth_ptx_deinit(void);

/**
* \brief Returns an ERROR message in a form defined by WPC Qi Auth Rel 1.3
* \param error_code[in]       Error code to be transmitted
* \param error_data[in]       Error data to be transmitted
* \param p_resp[out]          Output buffer
* \param resp_size[in][out]   Length of the buffer
*
* \retval Error Code
* \retval No Error - 0
*/
uint16_t qi_auth_ptx_error(uint8_t error_code, uint8_t error_data, uint8_t* p_resp, uint16_t* resp_size);


/**
* \brief Returns a DIGEST message in a form defined by  WPC Qi Auth Rel 1.3
* \param slot[in]          Slot number according to the specification
* \param p_resp[out]     Output buffer where the digest message will be stored
* \param resp_size[in][out]     Length of the buffer
*
* \retval Error Code
* \retval No Error - 0
*/
uint16_t qi_auth_ptx_digests(uint8_t slot, uint8_t* p_resp, uint16_t* resp_size);


/**
* \brief Returns a Certificate Chain in a form defined by WPC Qi Auth Rel 1.3
* \param slot[in]        Slot number
* \param offset          Offset within the chain to read the cert out
* \param p_resp[out]     Output buffer where the digest message will be stored
* \param resp_size[in][out]     Length of the buffer
*
* \retval Error Code
* \retval No Error - 0
*/
uint16_t qi_auth_ptx_certificate(uint8_t slot, uint8_t offset, uint8_t* p_resp, uint16_t* resp_size);


/**
* \brief Returns a Challenge message in a form defined by WPC Qi Auth Rel 1.3
* \param slot[in]         Slot number
* \param p_challreq[in]   Input Challenge request
* \param challreq_size[in] Length of the challenge
* \param p_resp[out] Output buffer for challenge response message
* \param resp_size[in][out] Length of the buffer
*
* \retval Error Code
* \retval No Error - 0
*/
uint16_t qi_auth_ptx_challenge_auth(uint8_t slot, uint8_t* p_challreq, uint16_t challreq_size,
		                                    uint8_t* p_resp, uint16_t* resp_size);


#endif
