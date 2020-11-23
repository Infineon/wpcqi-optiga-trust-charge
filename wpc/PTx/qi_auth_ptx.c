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
* \file    qi_auth_ptx.c
*
* \brief   This file implements the APIs, types and data structures for WPC Qi Auth Rel 1.3 implementation
*
*/

#include "qi_auth_ptx.h"
#include "qi_auth_ptx_crypt.h"
#include "optiga/common/optiga_lib_logger.h"


#define QI_AUTH_PROTO_VER                   0x1

#define MSGTYPE_RESP_DIGESTS                0x1
#define MSGTYPE_RESP_CERTIFICATE            0x2
#define MSGTYPE_RESP_CHALLENGE_AUTH         0x3
#define MSGTYPE_RESP_ERROR                  0x7

#define MAX_CERTIFICATE_OFFSET              (3*256 + 1023)
#define MAX_CERTIFICATE_LENGTH              (3*256 + 1023)

#define TRY_SLOT() \
	if (0 != slot) \
    { \
        return_status = PTX_BAD_SLOT;\
        break; \
    }

#define TRY_BUFFER() \
	if (NULL == p_resp) \
    { \
        return_status = PTX_BAD_BUFFER;\
        break; \
    }

#define TRY_SIZE() \
	if (0 == resp_size) \
    { \
        return_status = PTX_BAD_SIZE;\
        break; \
    }

uint16_t qi_auth_ptx_error(uint8_t error_code, uint8_t error_data, uint8_t* p_resp, uint16_t* resp_size)
{
    uint16_t return_status = PTX_ERROR;

    do {
    	TRY_BUFFER();
		TRY_SIZE();

        // Version and Message type
        p_resp[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_ERROR;

        // Error Code
        p_resp[1] = error_code;

        // Error Data
        p_resp[2] = error_data;

        *resp_size = 3;

        return_status = PTX_OK;

    }while(0);

    return return_status;
}

uint16_t qi_auth_ptx_digests(uint8_t slot, uint8_t* p_resp, uint16_t* resp_size)
{
    uint16_t return_status = PTX_ERROR;

    do {
        TRY_SLOT()
		TRY_BUFFER();
        TRY_SIZE();

        // Version and Message type
        p_resp[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_DIGESTS;

        // Slots Populated Mask and Slots Returned Mask
        // ToDo: This filed below should be updated based on the presence of Certificate CHips on your sample
        p_resp[1] = (0x1 << 4) | 0x1;

        return_status = qi_auth_ptx_crypt_certchain_sha256(slot, &p_resp[2]);
        if (return_status != PTX_OK)
        {
			return_status = PTX_CRYPT_HASH_FAILED;
			break;
		}

        *resp_size = 34;

        return_status = PTX_OK;

    }while(0);

    return return_status;
}

uint16_t qi_auth_ptx_certificate(uint8_t slot, uint8_t offset, uint8_t* p_resp, uint16_t* resp_size)
{
    uint16_t  return_status = PTX_ERROR;

    do {

    	TRY_SLOT()
        TRY_BUFFER();
        TRY_SIZE();

        // Version and Message type
        p_resp[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_CERTIFICATE;

        return_status = qi_auth_ptx_crypt_certchain(slot, offset, p_resp+1, resp_size);
        if (return_status != PTX_OK)
		{
			return_status = PTX_CRYPT_CERT_FAILED;
			break;
		}

        *resp_size += 1;

        return_status = PTX_OK;

    }while(0);

    return return_status;
}

uint16_t qi_auth_ptx_challenge_auth(uint8_t slot, uint8_t* p_challreq, uint16_t challreq_size,
                                            uint8_t* p_challresp, uint16_t* challresp_size)
{
    uint16_t return_status = PTX_ERROR;
    uint8_t certchain_hash[32];
    // To Be Signed data
    uint8_t tbs_auth[54] = {0x41};
    uint8_t digest_tbs_auth[32];
    //Signature
    uint8_t signature[64];
    uint16_t signature_len = 64;

    do {
    	TRY_SLOT();

        if ((NULL == p_challreq) || (NULL == p_challresp) || (NULL == challresp_size) )
        {
            return_status = PTX_BAD_BUFFER;
            break;
        }

        if ((0 == challreq_size) || (0 == *challresp_size))
        {
            return_status = PTX_BAD_SIZE;
            break;
        }

        *challresp_size = 0;

        // Version and Message type
        p_challresp[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_CHALLENGE_AUTH;
        *challresp_size += 1;

        // Maximum Qi Authentication Protocol Version[4] + SLots Populated Mask [4]
        // TODo: Adopt the below value to your security element configuration
        p_challresp[1] = (0x1 << 4) | 0x1;
        *challresp_size += 1;

        //Certificate Chain Hash LSB
        return_status = qi_auth_ptx_crypt_certchain_sha256(slot, certchain_hash);
        if (return_status != PTX_OK)
        {
        	return_status = PTX_CRYPT_HASH_FAILED;
        	break;
        }

        p_challresp[2] = certchain_hash[31];
        *challresp_size += 1;

        memcpy(&tbs_auth[1], certchain_hash, 32);

        // Table 15. B0 to B17 of the Challenge Request Message to which the CHALLENGE_AUTH is a response
        memcpy(&tbs_auth[1 + 32], p_challreq, 18);

        // Table 15. B0 to B2 of the Challenge Auth Response (Without the signature)
        memcpy(&tbs_auth[1 + 32 + 18], p_challresp, 3);

        // We need to hash TBS Auth Data and sign it afterwards
        qi_auth_ptx_crypt_generate_sha256(tbs_auth, 54, digest_tbs_auth);

        return_status = qi_auth_ptx_crypt_sign(digest_tbs_auth, 32, signature, &signature_len);
        if (return_status != PTX_OK)
        {
        	return_status = PTX_CRYPT_SIGN_FAILED;
        	break;
        }

        memcpy(&p_challresp[3], signature, signature_len);

        *challresp_size += signature_len;

        return_status = PTX_OK;

    }while(0);

    return return_status;
}

int32_t qi_auth_ptx_init(void)
{
	return (qi_auth_ptx_crypt_init(0));
}

int32_t qi_auth_ptx_deinit(void)
{
	return (qi_auth_ptx_crypt_deinit(0));
}
