/**
* MIT License
*
* Copyright (c) 2020-2023 Infineon Technologies AG
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
* \file    qi_auth_selftest.c
*
* \brief   This file uses PRx and PTx to test the correctness of the implementation
*
*/

#include "qi_auth_prx.h"
#include "qi_auth_ptx.h"
#include "qi_auth_fixtures.h"
#include "optiga/common/optiga_lib_logger.h"

extern void update_certificate_chain(void);

int qi_auth_self_test(void)
{
    uint8_t req[500];
    uint16_t req_size = 500;
    uint8_t resp[500];
    uint16_t resp_size = 500;

    uint8_t certchain[1000];
    uint16_t certchain_size = 1000;
    uint8_t rsid[9];
    uint16_t rsid_size = sizeof(rsid);
    uint8_t sha256[32];
    uint8_t pubkey[68];
    uint16_t pubkey_size = sizeof(pubkey);
    int status = 1;

    optiga_lib_print_string_with_newline("========= START TESTS =========");

    qi_auth_ptx_init();
    qi_auth_prx_init();

    // Some evaluation kits and samples have a wrong TEST Manufacturer CA and Root CA Populated
    // THis function will check and populate a new one if needed
    update_certificate_chain();

    do
    {
        // Test API
        // PRx: Prepare GET_DIGESTS message
        if (0 != qi_auth_prx_get_digests( 0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #1: Get Digest\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #1: Get Digest OK\r\n");
            optiga_lib_print_array_hex_format(req, req_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        // PTx: Prepare DIGESTS message
        if (0 != qi_auth_ptx_digests(0, resp, &resp_size))
        {
            optiga_lib_print_string_with_newline("Error #2: Get Digest response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #2: Get Digest response OK\r\n");
            optiga_lib_print_array_hex_format(resp, resp_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        // PRx: Prepare GET_CERTIFICATE message
        if (0 != qi_auth_prx_get_certificate(0, 0, 0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #3: Get Certificate\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #3: Get Certificate OK\r\n");
            optiga_lib_print_array_hex_format(req, req_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        //  PTx: Prepare CERTIFICATE message
        if (0 != qi_auth_ptx_certificate(0, 0, certchain, &certchain_size))
        {
            optiga_lib_print_string_with_newline("Error #4:  Get Certificate response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #4: Get Certificate response OK\r\n");
            optiga_lib_print_array_hex_format(certchain, certchain_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        //  PRx: Verify the CERTIFICATE message
        // We feed here the certificate without length at the beggining
        // Use qi_auth_test_new_certchain insetad of qi_auth_cr660_draft5_ifx_root_cert if you have generated a new certificate chain
        if (0 != qi_auth_prx_verify_cert(certchain+1+2, certchain_size-1-2, qi_auth_ifx_root_cert, sizeof(qi_auth_ifx_root_cert)))
        {
            optiga_lib_print_string_with_newline("Error #5: Verify Certificate response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #5: Verify Certificate response OK\r\n");
        }

        //  PRx: Prepare CHALLENGE message
        req_size = 18;
        if (0 != qi_auth_prx_challenge(0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #6: Get Challenge Request\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #6: Get Challenge Request OK\r\n");
            optiga_lib_print_array_hex_format(req, req_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        //  PTx: Feed the CHALLENGE message and prepare the CHALLENGE_AUTH message
        if (0 != qi_auth_ptx_challenge_auth(0, req, req_size, resp, &resp_size))
        {
            optiga_lib_print_string_with_newline("Error #7: Get Challenge Auth Response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #7: Get Challenge Auth Response OK\r\n");
            optiga_lib_print_array_hex_format(resp, resp_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        // PRx: extract some data to use it afterwards and not store the complete certificate
        // Here we take as input the CERTIFICATE message, which has one extra byte at the beggining
        qi_auth_prx_get_certchain_info(certchain + 1, certchain_size - 1,
                                       rsid, &rsid_size, sha256, pubkey, &pubkey_size);

        // PRx: Feed the CERTIFICATE, CHALLENGE used previously, and CHALLENGE_AUTH messages and verify the latter
        if (0 != qi_auth_prx_verify_chall_auth(sha256, pubkey, pubkey_size, req, req_size, resp, resp_size))
        {
            optiga_lib_print_string_with_newline("Error #8: Verify Challenge Auth Response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #8: Verify Challenge Auth Response OK\r\n");
        }

        // PRx: Test against fixtures
        if (0 != qi_auth_prx_verify_cert((uint8_t *)(qi_auth_13_test_cert_chain + 2), sizeof(qi_auth_13_test_cert_chain)-2,
                                         qi_auth_13_test_root_cert, sizeof(qi_auth_13_test_root_cert)))
        {
            optiga_lib_print_string_with_newline("Error #9:  Verify Certificate\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #9: Verify Certificate OK\r\n");
        }

        // PRx:
        qi_auth_prx_get_certchain_info((uint8_t *)qi_auth_13_test_cert_chain, sizeof(qi_auth_13_test_cert_chain),
                                             rsid, &rsid_size, sha256, pubkey, &pubkey_size);

        // PRx:
        if (0 != qi_auth_prx_verify_chall_auth( sha256, pubkey, pubkey_size,
                (uint8_t *)qi_auth_test_challenge, sizeof(qi_auth_test_challenge),
                (uint8_t *)qi_auth_test_challenge_auth, sizeof(qi_auth_test_challenge_auth)))
        {
            optiga_lib_print_string_with_newline("Error #10: Verify Challenge Auth\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #10: Verify Challenge Auth OK\r\n");
        }

        optiga_lib_print_string_with_newline("========= STOP TESTS =========");

        status = 0;
    }while(0);

    return status;
}
