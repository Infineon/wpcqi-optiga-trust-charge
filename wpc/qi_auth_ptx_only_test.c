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
* \file    qi_auth_selftest.c
*
* \brief   This file uses PRx and PTx to test the correctness of the implementation
*
*/

#include "qi_auth_ptx.h"
#include "qi_auth_fixtures.h"
#include "optiga/common/optiga_lib_logger.h"


int qi_auth_self_test(void)
{
#define WPC_CERTIFICATE_CHAIN_LENGTH 700
    uint8_t buffer[WPC_CERTIFICATE_CHAIN_LENGTH];
    uint16_t buffer_size = WPC_CERTIFICATE_CHAIN_LENGTH;
    int status;

    qi_auth_ptx_init();

    do
    {
        // Test API
        // PTx: Prepare DIGESTS message
        if (0 != qi_auth_ptx_digests(0, buffer, &buffer_size))
        {
        	// Error #1: Get Digest response
        	status = 1;
        }
        else
        {
        	// Test #1: Get Digest response OK
        	status = 1 << 4;
        }

        //  PTx: Prepare CERTIFICATE message
        buffer_size = WPC_CERTIFICATE_CHAIN_LENGTH;
        if (0 != qi_auth_ptx_certificate(0, 0, buffer, &buffer_size))
        {
        	// Error #2:  Get Certificate response
        	status |= 1 << 1;
        }
        else
        {
        	/* To save up some memory we don't do buffer rearrangement inside the
        	 * function, but here externally
        	 */
        	uint8_t tag = buffer[0];
        	buffer[4] = tag;
        	// &buffer[4] onwards of (buffer_size-4) contain the message
        	// Test #2: Get Certificate response OK
        	status |= 1 << 5;
        	//optiga_lib_print_array_hex_format(&buffer[4], (buffer_size-4), OPTIGA_LIB_LOGGER_COLOR_GREEN);
        }

        //  PTx: Feed the CHALLENGE message and prepare the CHALLENGE_AUTH message
        buffer_size = WPC_CERTIFICATE_CHAIN_LENGTH;
        if (0 != qi_auth_ptx_challenge_auth(0, (uint8_t *)qi_auth_test_challenge,
        									sizeof(qi_auth_test_challenge),
											buffer, &buffer_size))
        {
        	// Error #3: Get Challenge Auth Response
        	status |= 1 << 2;
        }
        else
        {
        	// Test #3: Get Challenge Auth Response OK
        	status |= 1 << 6;
        }

    }while(0);

    return status;
}
