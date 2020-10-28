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

#include "qi_auth_prx.h"
#include "qi_auth_ptx.h"
#include "qi_auth_fixtures.h"
#include "optiga/common/optiga_lib_logger.h"


int qi_auth_ptx_prx_test(void)
{
    uint8_t req[500];
    uint16_t req_size = 500;
    uint8_t resp[500];
    uint16_t resp_size = 500;

    uint8_t certchain[1000];
    uint16_t certchain_size = 1000;
    int status = 1;

    optiga_lib_print_string_with_newline("Start WPC Qi Auth testing routines\r\n");

    qi_auth_ptx_init();
    qi_auth_prx_init();

    do
    {
        // Test API
    	// Prepare GET_DIGESTS message
        if (0 != qi_auth_prx_get_digests(0, 0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #1: Get Digest\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #1: Get Digest OK\r\n");
            optiga_lib_print_array_hex_format(req, req_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        // Prepare DIGESTS message
        if (0 != qi_auth_ptx_digests(0, resp, &resp_size))
        {
            optiga_lib_print_string_with_newline("Error #2: Get Digest response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #2: Get Digest response OK\r\n");
            optiga_lib_print_array_hex_format(resp, resp_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        // Prepare GET_CERTIFICATE message
        if (0 != qi_auth_prx_get_certificate(0, 277, 0, 0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #3: Get Certificate\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #3: Get Certificate OK\r\n");
            optiga_lib_print_array_hex_format(req, req_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        // Prepare CERTIFICATE message
        if (0 != qi_auth_ptx_certificate(0, certchain, &certchain_size))
        {
            optiga_lib_print_string_with_newline("Error #4:  Get Certificate response\r\n");
            optiga_lib_print_array_hex_format(certchain, certchain_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #4: Get Certificate response OK\r\n");
        }

//        // Verify the CERTIFICATE message
//        if (0 != qi_auth_prx_verify_cert(certchain+1+2, certchain_size-1-2))
//        {
//            optiga_lib_print_string_with_newline("Error #5: Verify Certificate response\r\n");
//        }
//        else
//        {
//            optiga_lib_print_string_with_newline("Test #5: Verify Certificate response OK\r\n");
//        }

        // Prepare CHALLENGE message
        req_size = sizeof(req);
        if (0 != qi_auth_prx_challenge(0, 0, req, &req_size))
        {
            optiga_lib_print_string_with_newline("Error #6: Get Challenge Request\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #6: Get Challenge Request OK\r\n");
            optiga_lib_print_array_hex_format(req, req_size, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        // Feed the CHALLENGE message and prepare the CHALLENGE_AUTH message
        uint8_t resp_len;
        if (0 != qi_auth_ptx_challenge_auth(0, req, req_size, resp, &resp_len))
        {
            optiga_lib_print_string_with_newline("Error #7: Get Challenge Auth Response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #7: Get Challenge Auth Response OK\r\n");
            optiga_lib_print_array_hex_format(resp, resp_len, OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        // Feed the CERTIFICATE, CHALLENGE used previously, and CHALLENGE_AUTH messages and verify the latter
        if (0 != qi_auth_prx_verify_chall_auth(certchain+1, certchain_size-1, req, req_size, resp, resp_len))
        {
            optiga_lib_print_string_with_newline("Error #8: Verify Challenge Auth Response\r\n");
        }
        else
        {
            optiga_lib_print_string_with_newline("Test #8: Verify Challenge Auth Response OK\r\n");
        }

//        // Test against fixtures
//        if (0 != qi_auth_prx_verify_cert(qi_auth_cr660_draft5_certificate + 2, sizeof(qi_auth_cr660_draft5_certificate)-2))
//        {
//            optiga_lib_print_string_with_newline("Error #9:  Verify Certificate\r\n");
//        }
//        else
//        {
//            optiga_lib_print_string_with_newline("Test #9: Verify Certificate OK\r\n");
//        }
//
//        if (0 != qi_auth_prx_verify_chall_auth(    qi_auth_cr660_draft5_certificate, sizeof(qi_auth_cr660_draft5_certificate),
//                                                    qi_auth_cr660_draft5_challenge, sizeof(qi_auth_cr660_draft5_challenge),
//                                                    qi_auth_cr660_draft5_challenge_auth, sizeof(qi_auth_cr660_draft5_challenge_auth)))
//        {
//            optiga_lib_print_string_with_newline("Error #10: Verify Challenge Auth\r\n");
//        }
//        else
//        {
//            optiga_lib_print_string_with_newline("Test #10: Verify Challenge Auth OK\r\n");
//        }

        status = 0;
    }while(0);

    return status;
}
