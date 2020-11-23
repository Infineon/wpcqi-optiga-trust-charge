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
*
* \file    qi_auth_prx.c
*
* \brief   This file implements the APIs, types and data structures for WPC Qi Auth Rel 1.3 implementation
*
*/

#include "qi_auth_prx.h"
#include "qi_auth_prx_crypt.h"

#include "optiga/common/optiga_lib_logger.h"

#define QI_AUTH_PROTO_VER                   0x1

#define MSGTYPE_REQ_GET_DIGESTS             0x9
#define MSGTYPE_REQ_GET_CERTIFICATE         0xA
#define MSGTYPE_REQ_CHALLENGE               0xB
#define MSGTYPE_REQ_HANDOVER                0xC

#define MAX_CERTIFICATE_OFFSET              (3*256 + 1023)
#define MAX_CERTIFICATE_LENGTH              (3*256 + 1023)

#define TRY_SLOT() \
	if (0 != slot) \
    { \
        return_status = PRX_BAD_SLOT;\
        break; \
    }

#define TRY_BUFFER() \
	if (NULL == p_req) \
    { \
        return_status = PRX_BAD_BUFFER;\
        break; \
    }

#define TRY_SIZE() \
	if (0 == req_size) \
    { \
        return_status = PRX_BAD_SIZE;\
        break; \
    }


uint16_t qi_auth_prx_get_digests(uint8_t slot, uint8_t* p_req, uint16_t* req_size)
{
    uint16_t return_status = PRX_ERROR;

    do {
        TRY_SLOT();
        TRY_BUFFER();
        TRY_SIZE();

        // Version and Message type
        p_req[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_REQ_GET_DIGESTS;

        //Reserved [6] + Slot Mask[2]
        p_req[1] = (0x00) | (0x3 & slot) ;

        *req_size = 2;

        return_status = PRX_OK;

    }while(0);

    return return_status;
}

uint16_t qi_auth_prx_get_certificate(uint32_t offset, uint32_t length, uint8_t slot,
                                            uint8_t* p_req, uint16_t* req_size)
{
    uint16_t  return_status = PRX_ERROR;
    uint8_t offsetA8 = 0;
    uint8_t offset70 = 0;
    uint8_t lengthA8 = 0;
    uint8_t length70 = 0;

    do {

        if ((offset > MAX_CERTIFICATE_OFFSET) || (length > MAX_CERTIFICATE_OFFSET))
        {
            return_status = PRX_BAD_LENGTH;
            break;
        }

        TRY_SLOT();
        TRY_BUFFER();
        TRY_SIZE();

        offsetA8 = offset >> 8;
        offset70 = offset - (offsetA8 << 8);

        lengthA8 = length >> 8;
        length70 = length - (lengthA8 << 8);

        // Version and Message type
        p_req[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_REQ_GET_CERTIFICATE;

        // OffsetA8[3]+ LengthA8[3]  + Slot Number [2]
        p_req[1] = ((0x7 & offsetA8) << 5) | ((0x7 & lengthA8) << 2) | (0x3 & slot);

        // Offset70
        p_req[2] =  offset70;

        // Length70
        p_req[3] =  length70;

        *req_size = 4;

        return_status = PRX_OK;

    }while(0);

    return return_status;
}

uint16_t qi_auth_prx_challenge(uint8_t slot, uint8_t* p_req, uint16_t* req_size)
{
    uint16_t return_status = PRX_ERROR;

    do {
        TRY_SLOT();
        TRY_BUFFER();
        TRY_SIZE();

        // Version and Message type
        p_req[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_REQ_CHALLENGE;

        //Reserved [6] + Slot Mask[2]
        p_req[1] = (0x00) | (0x3 & slot) ;

        //128bit Nonce
        return_status = qi_auth_prx_crypt_get_random(16, &p_req[2], *req_size - 2);
        if (return_status != CRYPT_LIB_OK)
            break;

        *req_size = 18;

        return_status = PRX_OK;

    }while(0);

    return return_status;
}

uint16_t qi_auth_prx_verify_chall_auth(uint8_t* p_sha256,
		                               uint8_t* p_puc_pubkey, uint16_t pubkey_size,
                                       uint8_t* p_chall, uint16_t chall_size,
                                       uint8_t* p_challauth, uint16_t challauth_size)
{
    uint16_t return_status = PRX_ERROR;
    signature_container_t sign_vector;
    uint8_t signed_msg[54] = {0x41};
    //uint8_t digest_signed_msg[32];

    do {

        if ((NULL == p_sha256) || (NULL == p_puc_pubkey) ||
            (NULL == p_chall) || (0 == chall_size) ||
            (NULL == p_challauth) || (0 == challauth_size))
            break;

        //Getting the signature
        sign_vector.p_signature = &p_challauth[3];
        sign_vector.signature_size = 64;

        //Getting the Public key
        sign_vector.p_public_key = p_puc_pubkey;
        sign_vector.public_key_size = pubkey_size;

        //Calculating hash of the chain
        memcpy(&signed_msg[1], p_sha256, 32);

        // Copy Challenge Request
        memcpy(&signed_msg[1 + 32], p_chall, 18);

        // Copy Challenge Auth response
        memcpy(&signed_msg[1 + 32 + 18], p_challauth, 3);

        sign_vector.p_message = signed_msg;
        sign_vector.message_size = sizeof(signed_msg);

        return_status = qi_auth_prx_crypt_verify_signature(&sign_vector);
        if(CRYPT_LIB_OK != return_status)
        {
            break;
        }
        return_status = PRX_OK;
    }while(0);

    return return_status;
}

uint16_t qi_auth_prx_verify_cert( uint8_t* p_certchain, uint16_t chain_size, const uint8_t* p_root_ca, uint16_t root_ca_size )
{
    uint16_t return_status = PRX_ERROR;

    do {
        return_status = qi_auth_prx_crypt_verify_certchain(p_certchain, chain_size, p_root_ca, root_ca_size);
        if(CRYPT_LIB_OK != return_status)
        {
            break;
        }

        return_status = PRX_OK;

    }while(0);

    return return_status;
}

uint16_t qi_auth_prx_get_certchain_info(uint8_t* p_certchain, uint16_t chain_size,
 		                                uint8_t* p_puc_rsid, uint16_t* p_puc_rsid_size,
 									    uint8_t* p_sha256,
 									    uint8_t* p_puc_pubkey, uint16_t* p_puc_pubkey_size)
{
    uint16_t return_status = PRX_ERROR;

    do {
        return_status = qi_auth_prx_crypt_get_certchain_info(p_certchain, chain_size,
                                                             p_puc_rsid, p_puc_rsid_size,
				                                             p_sha256,
				                                             p_puc_pubkey, p_puc_pubkey_size);
        if(CRYPT_LIB_OK != return_status)
        {
            break;
        }

        return_status = PRX_OK;

    }while(0);

    return return_status;
}

int32_t qi_auth_prx_init(void)
{
	return (qi_auth_prx_crypt_init(0));
}

int32_t qi_auth_prx_deinit(void)
{
	return (qi_auth_prx_crypt_deinit(0));
}

