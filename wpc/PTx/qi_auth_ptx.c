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

uint16_t qi_auth_ptx_error(uint8_t error_code, uint8_t error_data, uint8_t* p_resp, uint16_t* resp_size)
{
    uint16_t return_status = CRYPT_LIB_ERROR;

    do {
        if (NULL == p_resp)
            break;

        if (0 == resp_size)
            break;

        // Version and Message type
        p_resp[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_ERROR;

        // Error Code
        p_resp[1] = error_code;

        // Error Data
        p_resp[2] = error_data;

        *resp_size = 3;

        return_status = CRYPT_LIB_OK;

    }while(0);

    return return_status;
}

uint16_t qi_auth_ptx_digests(uint8_t slot, uint8_t* p_digest, uint16_t* digest_size)
{
    uint16_t return_status = CRYPT_LIB_ERROR;

    do {
        if (NULL == p_digest)
            break;

        if (0 == digest_size)
            break;

        if (0 != slot)
            break;

        // Version and Message type
        p_digest[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_DIGESTS;

        // Slots Populated Mask and Slots Returned Mask
        p_digest[1] = 0x1 | 0x1;

        return_status = qi_auth_ptx_crypt_certchain_sha256(&p_digest[2]);
        if (return_status != CRYPT_LIB_OK)
            break;

        *digest_size = 34;

    }while(0);

    return return_status;
}

uint16_t qi_auth_ptx_certificate(uint8_t slot, uint8_t* p_certchain, uint16_t* certchain_size)
{
    uint16_t  return_status = CRYPT_LIB_ERROR;

    do {

        if ((0 != slot) || (NULL == p_certchain) || (0 == certchain_size))
            break;

        // Version and Message type
        p_certchain[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_CERTIFICATE;

        return_status = qi_auth_ptx_crypt_certchain(p_certchain+1, certchain_size);
        if (return_status != CRYPT_LIB_OK)
            break;

        *certchain_size += 1;

    }while(0);

    return return_status;
}

uint16_t qi_auth_ptx_challenge_auth(uint8_t slot, uint8_t* p_challreq, uint8_t challreq_size,
                                            uint8_t* p_challresp, uint8_t* challresp_size)
{
    uint16_t return_status = CRYPT_LIB_ERROR;
    uint8_t certchain_hash[32];
    // To Be Signed data
    uint8_t tbs_auth[54] = {0x41};
    uint8_t digest_tbs_auth[32];
    //Signature
    uint8_t signature[64];
    uint16_t signature_len = 64;

    do {
        if ((0 != slot) ||
            (NULL == p_challreq) || (0 == challreq_size) ||
            (NULL == p_challresp) || (NULL == challresp_size) || (0 == *challresp_size))
            break;

        *challresp_size = 0;

        // Version and Message type
        p_challresp[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_CHALLENGE_AUTH;
        *challresp_size += 1;

        // Maximum Qi Authentication Certificate Structure Version
        p_challresp[1] = 0x1 | 0x1;
        *challresp_size += 1;

        //Certificate Chain Hash LSB
        return_status = qi_auth_ptx_crypt_certchain_sha256(certchain_hash);
        if (return_status != CRYPT_LIB_OK)
            break;

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
        if (return_status != CRYPT_LIB_OK)
            break;

        memcpy(&p_challresp[3], signature, signature_len);

        *challresp_size += signature_len;

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
