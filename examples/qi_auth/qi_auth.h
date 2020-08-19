/**
* MIT License
*
* Copyright (c) 2018 Infineon Technologies AG
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
* \file    qi_auth_.h
*
* \brief   This file defines APIs, types and data structures for WPC Qi Auth Rel 1.3 implementation.
*
* @{
*/
#ifndef _QI_AUTH_H_
#define _QI_AUTH_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#define QI_AUTH_ERROR_INVALID_REQUEST			0x01
#define QI_AUTH_ERROR_UNSUPPORTED_PROTOCOL		0x02
#define QI_AUTH_ERROR_BUSY						0x03
#define QI_AUTH_ERROR_UNSPECIFIED				0x04
#define QI_AUTH_ERROR_COMMUNCIATION_ERROR		0x01

/**
* \brief Returns an ERROR message in a form defined by WPC Qi Auth Rel 1.3
* \param error_code[in]       Error code to be transmitted
* \param error_data[in]       Error data to be transmitted
* \param p_resp[out]          Output buffer
* \param resp_size[in][out]   Length of the buffer
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_resp_error(uint8_t error_code, uint8_t error_data, uint8_t* p_resp, uint16_t* resp_size);

/**
* \brief Returns a GET DIGESTS message in a form defined by WPC Qi Auth Rel 1.3
* \param ptx[in]		  A value of 0 indicates that the Initiator is a PRx. A value of 1 indicates that the Initiator is a PTx.
* \param slot[in]         Slot number
* \param p_req[out]       Output buffer
* \param req_size[in][out]    Length of the buffer
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_req_get_digests(uint8_t ptx, uint8_t slot, uint8_t* p_req, uint16_t* req_size);


/**
* \brief Returns a DIGEST message in a form defined by  WPC Qi Auth Rel 1.3
* \param slot[in]          Slot number according to Draft specification
* \param p_resp[out]     Output buffer where the digest message will be stored
* \param resp_size[in][out]     Length of the buffer
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_resp_digests(uint8_t slot, uint8_t* p_resp, uint16_t* resp_size);

/**
* \brief Returns a GET_CERTIFICATE message in a form defined by WPC Qi Auth Rel 1.3
* \param offset[in]       Offset.	These two fields combine to form the offset in bytes from
*                         the start of the Certificate Chain to where the read request begins offset value
* \param length[in]       Length in bytes to read. The length value is length98*256+length70. It is an error to specify a length that
*                         would result in reading beyond end of the Certificate Chain
* \param ptx[in]          A value of 0 indicates that the Initiator is a PRx. A value of 1 indicates that the Initiator is a PTx.
* \param slot[in]	      Slot number
* \param p_req[out]       Output buffer
* \param req_size[in][out]    Length of the buffer
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_req_get_certificate(uint32_t offset, uint32_t length, uint8_t ptx, uint8_t slot,
		                                    uint8_t* p_req, uint16_t* req_size);

/**
* \brief Returns a Certificate Chain in a form defined by WPC Qi Auth Rel 1.3
* \param slot[in]          Slot number
* \param offset[in]        Offset
* \param length[in]        Length of the Certificate
* \param p_resp[out]     Output buffer where the digest message will be stored
* \param resp_size[in][out]     Length of the buffer
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_resp_certificate(uint8_t slot, uint16_t offset, uint16_t length,
		                                 uint8_t* p_resp, uint16_t* resp_size);

/**
* \brief Returns a Challenge message in a form defined by WPC Qi Auth Rel 1.3
* \param ptx[in]          A value of 0 indicates that the Initiator is a PRx. A value of 1 indicates that the Initiator is a PTx.
* \param slot[in]         Slot number
* \param p_req[out]       Output buffer
* \param req_size[in][out]    Length of the buffer
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_req_challenge(uint8_t ptx, uint8_t slot, uint8_t* p_creq, uint16_t* req_size);

/**
* \brief Returns a Challenge message in a form defined by WPC Qi Auth Rel 1.3
* \param slot[in]         Slot number
* \param p_challreq[in]   Input Challenge request
* \param challreq_size[in] Length of the challenge
* \param p_resp[out] Output buffer for challenge response message
* \param resp_size[in][out] Length of the buffer
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_resp_challenge_auth(uint8_t slot, uint8_t* p_challreq, uint8_t challreq_size,
		                                    uint8_t* p_resp, uint8_t* resp_size);


/**
* \brief Returns a HANDOVER mesage in a form defined by WPC Qi Auth Rel 1.3 
* \param p_req[out]       Output buffer
* \param req_size[in][out]    Length of the buffer
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_req_handover(uint8_t* p_req, uint16_t* req_size);

/**
* \brief Verifies a given Response message in a form defined by WPC Qi Auth Rel 1.3. Challenge auth message is given as an input
* \param p_challreq[in]     Challenge request message
* \param challreq_size[in]  Challenge request message size
* \param p_challresp[in]    Challenge response message
* \param challresp_size[in] Challenge response message size
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_verify_chall_auth(uint8_t* p_certchain, uint16_t certchain_size,
		                                  uint8_t* p_challreq, uint16_t challreq_size,
		                                  uint8_t* p_challresp, uint16_t challresp_size);

/**
* \brief Verifies a given Certificate Chain message in a form defined by WPC Qi Auth Rel 1.3. Certificate Chain is given as an input
* \param p_certchain[in]  Certificate Chain message
* \param chain_size[in]   Chain size
*
* \retval Error Code
* \retval #CRYPTO_LIB_SUCCESS
*/
uint16_t qi_auth_verify_cert(uint8_t* p_certchain, uint16_t chain_size);


/**
* \brief Testing the WPC Auth command
*
*/
int qi_auth_test(void);

#endif
