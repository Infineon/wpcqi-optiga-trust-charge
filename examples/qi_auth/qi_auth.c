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
*
*
* \file    qi_auth_.c
*
* \brief   This file implements the APIs, types and data structures for WPC Qi Auth Rel 1.3 implementation
*
*/

#include "optiga/common/optiga_lib_logger.h"

#include "qi_auth.h"
#include "qi_auth_crypt.h"

#define QI_AUTH_PROTO_VER                   0x1

#define MSGTYPE_RESP_DIGESTS                0x1
#define MSGTYPE_RESP_CERTIFICATE            0x2
#define MSGTYPE_RESP_CHALLENGE_AUTH         0x3
#define MSGTYPE_RESP_ERROR                  0x7

#define MSGTYPE_REQ_GET_DIGESTS             0x9
#define MSGTYPE_REQ_GET_CERTIFICATE         0xA
#define MSGTYPE_REQ_CHALLENGE               0xB
#define MSGTYPE_REQ_HANDOVER                0xC

#define MAX_CERTIFICATE_OFFSET              (3*256 + 1023)
#define MAX_CERTIFICATE_LENGTH              (3*256 + 1023)


uint8_t qi_auth_rel13_spec_certificate[] =
{
        0x01, 0x14,
    // Root Hash
        0xBB, 0xEC, 0x6A, 0xE8, 0x0D, 0x77, 0x64, 0xE3, 0x8F, 0x22, 0xF5, 0x32, 0xF3, 0xE1, 0x43, 0x20,
        0xC6, 0x1F, 0xE2, 0x81, 0x2A, 0xD6, 0xA9, 0x34, 0x9C, 0x30, 0xE2, 0x9E, 0x13, 0x03, 0xFD, 0xF5,
    // Interim Cert
        0x01,
        0x02,
        0x39,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        // Issuer ID
        0x57, 0x50,    0x43, 0x00, 0x00, 0x00, // WPC\0
        // Subject ID
        0x4D, 0x3A, 0x43, 0x41, 0x43, 0x41, // M:CACA
        // Public Key
        0x02,
        0xA4, 0xC2, 0x6A, 0xED, 0x81, 0xAD, 0x9C, 0x75, 0xB4, 0xD0, 0x6C, 0x5B, 0x75, 0xAB, 0x9F, 0x29,
        0x30, 0x21, 0xF4, 0x6A, 0x8F, 0x97, 0x05, 0x57, 0xF5, 0x4F, 0xD9, 0x6D, 0x11, 0xF3, 0x25, 0xF9,
        // Signature
        0x02, 0x17, 0xE6, 0x17, 0xF0, 0xB6, 0x44, 0x39, 0x28, 0x27, 0x8F, 0x96, 0x99, 0x9E, 0x69, 0xA2,
        0x3A, 0x4F, 0x2C, 0x15, 0x2B, 0xDF, 0x6D, 0x6C, 0xDF, 0x66, 0xE5, 0xB8, 0x02, 0x82, 0xD4, 0xED,
        0x1B, 0xEB, 0xD1, 0x42, 0x20, 0x69, 0x76, 0x84, 0x19, 0xD7, 0x5A, 0x4B, 0xF7, 0xEC, 0xA6, 0xA7,
        0x93, 0x61, 0x2F, 0x6E, 0xEC, 0x1C, 0x1F, 0x12, 0xE1, 0x7B, 0x0B, 0x2D, 0xAA, 0xD1, 0xEB, 0xA9,
    // Leaf Cert
        0x01,
        0x07,
        0x39,
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0xD2,
        // Issuer ID
        0x4D, 0x3A, 0x43, 0x41, 0x43, 0x41, // M:CACA
        // Subject ID
        0x00, 0x63, 0x86, 0x00, 0x00, 0x00,
        // Public Key
        0x03,
        0x37, 0xAE, 0x71, 0x7C, 0xFD, 0x8C, 0xBC, 0x95, 0x01, 0x5A, 0x1A, 0x6E, 0x76, 0xDF, 0x8D, 0x6F,
        0x7F, 0x4B, 0x8D, 0x77, 0xD2, 0x13, 0xCC, 0x5F, 0x82, 0x44, 0x4E, 0x85, 0x53, 0xF6, 0xD0, 0x78,
        // Signature
        0x02, 0x17, 0xE6, 0x17, 0xF0, 0xB6, 0x44, 0x39, 0x28, 0x27, 0x8F, 0x96, 0x99, 0x9E, 0x69, 0xA2,
        0x3A, 0x4F, 0x2C, 0x15, 0x2B, 0xDF, 0x6D, 0x6C, 0xDF, 0x66, 0xE5, 0xB8, 0x02, 0x82, 0xD4, 0xED,
        0x08, 0xD5, 0x29, 0xF0, 0x4A, 0x1F, 0x30, 0x9B, 0xDF, 0xEE, 0x6E, 0xC6, 0x08, 0x64, 0x83, 0x3C,
        0x39, 0x71, 0x3F, 0x82, 0x17, 0x46, 0xC4, 0x4B, 0x13, 0xAD, 0xF3, 0x7B, 0x84, 0x3F, 0xEA, 0x03,
};

uint8_t qi_auth_rel13_spec_challenge[] = {
        0x1B, 0x00, 0xA5, 0x02, 0x3B, 0x84, 0xB4, 0x6C, 0xD7, 0x10, 0x2E, 0x6F, 0x51, 0x9A, 0xF7, 0x99,
        0x37, 0x52
};

uint8_t qi_auth_rel13_spec_challenge_auth[] = {
        0x13, 0x11, 0x0C, 0x02, 0x17, 0xE6, 0x17, 0xF0, 0xB6, 0x44, 0x39, 0x28, 0x27, 0x8F, 0x96, 0x99,
        0x9E, 0x69, 0xA2, 0x3A, 0x4F, 0x2C, 0x15, 0x2B, 0xDF, 0x6D, 0x6C, 0xDF, 0x66, 0xE5, 0xB8, 0x02,
        0x82, 0xD4, 0xED, 0x21, 0x5F, 0x0D, 0xD6, 0xEF, 0x65, 0xFE, 0x25, 0x53, 0x4B, 0xA9, 0x3C, 0x21,
        0x80, 0x4E, 0xB0, 0x14, 0xB4, 0x2E, 0xBB, 0x6D, 0x89, 0xFC, 0x69, 0x1F, 0x47, 0xE0, 0xC0, 0x16,
        0xD6, 0x99, 0x33
};




uint16_t qi_auth_resp_error(uint8_t error_code, uint8_t error_data, uint8_t* p_resp, uint16_t* resp_size)
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

uint16_t qi_auth_req_get_digests(uint8_t ptx, uint8_t slot, uint8_t* p_req, uint16_t* req_size)
{
    uint16_t return_status = CRYPT_LIB_ERROR;

    do {
        if (0 != slot)
            break;

        if (NULL == p_req)
            break;

        if (0 == req_size)
            break;

        // Version and Message type
        p_req[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_REQ_GET_DIGESTS;

        //Reserved [3] + PTX[1] + Slot Mask[4]
        p_req[1] = (ptx << 4) | (0xf & slot) ;

        *req_size = 2;

        return_status = CRYPT_LIB_OK;

    }while(0);

    return return_status;
}


uint16_t qi_auth_resp_digests(uint8_t slot, uint8_t* p_digest, uint16_t* digest_size)
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

        return_status = qi_auth_crypt_certchain_sha256(&p_digest[2]);
        if (return_status != CRYPT_LIB_OK)
            break;

        *digest_size = 34;

    }while(0);

    return return_status;
}

uint16_t qi_auth_req_get_certificate(uint32_t offset, uint32_t length, uint8_t ptx, uint8_t slot,
                                            uint8_t* p_req, uint16_t* req_size)
{
    uint16_t  return_status = CRYPT_LIB_ERROR;
    uint8_t offset98 = 0;
    uint8_t offset70 = 0;
    uint8_t length98 = 0;
    uint8_t length70 = 0;

    do {

        if ((offset > MAX_CERTIFICATE_OFFSET) || (length > MAX_CERTIFICATE_OFFSET) ||
            (0 != slot) || (NULL == p_req) || (0 == req_size))
            break;

        offset98 = offset >> 8;
        offset70 = offset - (offset98 << 8);

        length98 = length >> 8;
        length70 = length - (length98 << 8);

        // Version and Message type
        p_req[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_REQ_GET_CERTIFICATE;

        // Offset98[2]+ Length98[2] + Reserved[1] + PTx[1] + Slot Number [2]
        p_req[1] = ((0x3 & offset98) << 6) | ((0x3 & length98) << 4) |
                    (0 << 3) | ((0x1 & ptx) << 3) | (0x3 & slot);

        // Offset70
        p_req[2] =  offset70;

        // Length70
        p_req[3] =  length70;

        *req_size = 4;

        return_status = CRYPT_LIB_OK;

    }while(0);

    return return_status;
}


uint16_t qi_auth_resp_certificate(uint8_t slot, uint16_t offset, uint16_t length,
                              uint8_t* p_certchain, uint16_t* certchain_size)
{
    uint16_t  return_status = CRYPT_LIB_ERROR;

    do {

        if ((0 != slot) || (NULL == p_certchain) || (0 == certchain_size))
            break;

        // Version and Message type
        p_certchain[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_RESP_CERTIFICATE;

        return_status = qi_auth_crypt_certchain(p_certchain+1, certchain_size);
        if (return_status != CRYPT_LIB_OK)
            break;

        *certchain_size += 1;

    }while(0);

    return return_status;
}


uint16_t qi_auth_req_challenge(uint8_t ptx, uint8_t slot, uint8_t* p_req, uint16_t* req_size)
{
    uint16_t return_status = CRYPT_LIB_ERROR;

    do {
        if (0 != slot)
            break;

        if (NULL == p_req)
            break;

        if ((NULL == req_size) || ( 2 > *req_size))
            break;

        // Version and Message type
        p_req[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_REQ_CHALLENGE;

        //Reserved [3] + PTX[1] + Slot Mask[4]
        p_req[1] = (ptx << 4) | (0xf & slot) ;

        //128bit Nonce
        return_status = qi_auth_crypt_get_random(16, &p_req[2], *req_size - 2);
        if (return_status != CRYPT_LIB_OK)
            break;

        *req_size = 18;

    }while(0);

    return return_status;
}

uint16_t qi_auth_resp_challenge_auth(uint8_t slot, uint8_t* p_challreq, uint8_t challreq_size,
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
        return_status = qi_auth_crypt_certchain_sha256(certchain_hash);
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
        qi_auth_crypt_generate_sha256(tbs_auth, 54, digest_tbs_auth);

        return_status = qi_auth_crypt_sign(digest_tbs_auth, 32, signature, &signature_len);
        if (return_status != CRYPT_LIB_OK)
            break;

        memcpy(&p_challresp[3], signature, signature_len);

        *challresp_size += signature_len;

    }while(0);

    return return_status;
}


uint16_t qi_auth_req_handover(uint8_t* p_req, uint16_t* req_size)
{
    uint16_t return_status = CRYPT_LIB_ERROR;

    do {
        if (NULL == p_req)
            break;

        if (0 == req_size)
            break;

        // Version and Message type
        p_req[0] = (QI_AUTH_PROTO_VER << 4) | MSGTYPE_REQ_HANDOVER;

        *req_size = 1;

        return_status = CRYPT_LIB_OK;

    }while(0);

    return return_status;
}


uint16_t qi_auth_verify_chall_auth(uint8_t* p_certchain, uint16_t certchain_size,
                                          uint8_t* p_chall, uint16_t chall_size,
                                          uint8_t* p_challauth, uint16_t challauth_size)
{
    uint16_t return_status = CRYPT_LIB_ERROR;
    signature_container_t sign_vector;
    uint8_t signed_msg[54] = {0x41};
    //uint8_t digest_signed_msg[32];

    do {

        if ((NULL == p_certchain) || (0 == certchain_size) ||
            (NULL == p_chall) || (0 == chall_size) ||
            (NULL == p_challauth) || (0 == challauth_size))
            break;

        //Getting the signature
        sign_vector.p_signature = &p_challauth[3];
        sign_vector.signature_size = 64;

        //Getting the Public key
        sign_vector.p_public_key = &p_certchain[QI_AUTH_CRT_ROOTHASH_LEN + QI_AUTH_CRT_LEN + QI_AUTH_CRT_PUBKEY_OFF+2];
        sign_vector.public_key_size = 33;

        //Calculating hash of the chain
        qi_auth_crypt_generate_sha256(p_certchain, certchain_size, &signed_msg[1]);

        // Copy Challenge Request
        memcpy(&signed_msg[1 + 32], p_chall, 18);

        // Copy Challenge Auth response
        memcpy(&signed_msg[1 + 32 + 18], p_challauth, 3);

        sign_vector.p_message = signed_msg;
        sign_vector.message_size = sizeof(signed_msg);

        return_status = qi_auth_crypt_verify_signature(&sign_vector);
        if(CRYPT_LIB_OK != return_status)
        {
            break;
        }
        return_status = CRYPT_LIB_OK;
    }while(0);

    return return_status;
}


uint16_t qi_auth_verify_cert(uint8_t* p_certchain, uint16_t chain_size)
{
    uint16_t return_status = CRYPT_LIB_ERROR;
    uint8_t raw_pubkey[0x21];

    do {
        return_status = qi_auth_crypt_verify_certchain(p_certchain, chain_size, raw_pubkey, 0x21);
        if(CRYPT_LIB_OK != return_status)
        {
            break;
        }

        return_status = CRYPT_LIB_OK;

    }while(0);

    return return_status;
}


int qi_auth_test(void)
{
    uint8_t req[500];
    uint16_t req_size = 500;
    uint8_t resp[500];
    uint16_t resp_size = 500;

    uint8_t certchain[300];
    uint16_t certchain_size = 300;
    int status = 1;

    optiga_lib_print_string_with_newline("Start WPC Qi Auth testing routines\r\n");

    qi_auth_crypt_init();

    do
    {
        // Test API
        // Test against external records
        if (CRYPT_LIB_OK != qi_auth_req_get_digests(0, 0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #1: Get Digest\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #1: Get Digest OK\r\n");
        }

        if (CRYPT_LIB_OK != qi_auth_resp_digests(0, resp, &resp_size))
        {
            optiga_lib_print_string_with_newline("Error #2: Get Digest response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #2: Get Digest response OK\r\n");
        }



        if (CRYPT_LIB_OK != qi_auth_req_get_certificate(0, 277, 0, 0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #3: Get Certificate\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #3: Get Certificate OK\r\n");
        }


        if (CRYPT_LIB_OK != qi_auth_resp_certificate(0, 0, 277, certchain, &certchain_size))
        {
            optiga_lib_print_string_with_newline("Error #4:  Get Certificate response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #4: Get Certificate response OK\r\n");
        }

        if (CRYPT_LIB_OK != qi_auth_verify_cert(certchain+1+2, certchain_size-1-2))
        {
            optiga_lib_print_string_with_newline("Error #5: Verify Certificate response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #5: Verify Certificate response OK\r\n");
        }

        req_size = sizeof(req);
        if (CRYPT_LIB_OK != qi_auth_req_challenge(0, 0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #6: Get Challenge Request\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #6: Get Challenge Request OK\r\n");
        }

        uint8_t resp_len;
        if (CRYPT_LIB_OK != qi_auth_resp_challenge_auth(0, req, req_size, resp, &resp_len))
        {
            optiga_lib_print_string_with_newline("Error #7: Get Challenge Auth Response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #7: Get Challenge Auth Response OK\r\n");
        }


        if (CRYPT_LIB_OK != qi_auth_verify_chall_auth(certchain+1, certchain_size-1, req, req_size, resp, resp_len))
        {
            optiga_lib_print_string_with_newline("Error #8: Verify Challenge Auth Response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #8: Verify Challenge Auth Response OK\r\n");
        }

        // Test against external records
        // to remove the extra two bytes
        if (CRYPT_LIB_OK != qi_auth_verify_cert(qi_auth_rel13_spec_certificate + 2, sizeof(qi_auth_rel13_spec_certificate)-2))
        {
            optiga_lib_print_string_with_newline("Error #9:  Verify Certificate\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #9: Verify Certificate OK\r\n");
        }

        if (CRYPT_LIB_OK != qi_auth_verify_chall_auth(    qi_auth_rel13_spec_certificate, sizeof(qi_auth_rel13_spec_certificate),
                                                    qi_auth_rel13_spec_challenge, sizeof(qi_auth_rel13_spec_challenge),
                                                    qi_auth_rel13_spec_challenge_auth, sizeof(qi_auth_rel13_spec_challenge_auth)))
        {
            optiga_lib_print_string_with_newline("Error #10: Verify Challenge Auth\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #10: Verify Challenge Auth OK\r\n");
        }

        status = CRYPT_LIB_OK;
    }while(0);

    return status;
}
