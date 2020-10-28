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
* \file qi_auth_ptx_crypt.c
*
* \brief   This file implements the APIs for Crypto Wrappers.
* @{
*/

#include <string.h>

#include <optiga/optiga_crypt.h>
#include <optiga/optiga_util.h>
#include <optiga/pal/pal_crypt.h>
#include <optiga/pal/pal_os_event.h>
#include <optiga/pal/pal_os_timer.h>
#include <optiga/common/optiga_lib_logger.h>
#include <ecdsa_utils.h>

#include "qi_auth_ptx_crypt.h"
#include <ctype.h>

#define MAXCMD_LEN			255
#define HEXDUMP_COLS    	16


#define QI_AUTH_CERTCAIN_OID	                0xE0E0

#define QI_AUTH_DEVICE_PRIKEY_OID               OPTIGA_KEY_ID_E0F0

/// Used to check ZERO Length
#define ZERO_LENGTH                         0x00

///Maximum length of the generated seed
#define MAX_SEED_LENGTH                     0x20

/// Value to indicate that the random bytes are read from Security Chip
#define RANDOM_READ                         0xAA

/// Value to indicate that the RNG is initialized
#define RNG_INITIALIZED                     0x95

///Number of bytes of random data read from Security Chip
#define RANDOM_DATA_LENGTH                  0x18

///Size of the counter
#define COUNTER_LENGTH            	        0x08

///size of public key for NIST-P256
#define LENGTH_PUB_KEY   			        0x21

/// @endcond

/// Result is OK
#define RESULT_OK                           0x00


#ifdef OPTIGA_LIB_ENABLE_COMMS_LOGGING
#define WAIT_FOR_COMPLETION(ret) \
	if (OPTIGA_LIB_SUCCESS != ret)\
	{\
		break;\
	}\
	while (optiga_lib_status == OPTIGA_LIB_BUSY) \
	{\
		pal_os_timer_delay_in_milliseconds(1);\
		pal_os_event_trigger_registered_callback();\
	} \
	\
	if (OPTIGA_LIB_SUCCESS != optiga_lib_status)\
	{ \
			ret = optiga_lib_status;\
			printf("Error: 0x%02X \r\n", optiga_lib_status);\
			optiga_util_destroy(p_util);\
			optiga_crypt_destroy(p_crypt);\
			pal_os_event_destroy(NULL);\
			break; \
	}
#else
#define WAIT_FOR_COMPLETION(ret) \
	if (OPTIGA_LIB_SUCCESS != ret)\
	{\
		break;\
	}\
	while (optiga_lib_status == OPTIGA_LIB_BUSY) \
	{\
		pal_os_timer_delay_in_milliseconds(1);\
	} \
	\
	if (OPTIGA_LIB_SUCCESS != optiga_lib_status)\
	{ \
			ret = optiga_lib_status;\
			printf("Error: 0x%02X \r\n", optiga_lib_status);\
			optiga_util_destroy(p_util);\
			optiga_crypt_destroy(p_crypt);\
			pal_os_event_destroy(NULL);\
			break; \
	}
#endif

#define CHECK_RESULT(expr) \
	return_status = (int32_t)OPTIGA_DEVICE_ERROR; \
	optiga_lib_status = OPTIGA_LIB_BUSY;\
	return_status = expr;\
	WAIT_FOR_COMPLETION(return_status);\

static volatile optiga_lib_status_t optiga_lib_status;
static optiga_crypt_t* p_crypt = NULL;
static optiga_util_t* p_util = NULL;

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static void optiga_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
}



static uint16_t optiga_init(uint8_t restore_from_hibernate)
{
	uint16_t return_status = 1;

	do
	{
		//Create an instance of optiga_util to open the application on OPTIGA.
		p_util = optiga_util_create(0, optiga_callback, NULL);

		//Create an instance of optiga_crypt to perform crypto operations on OPTIGA.
		p_crypt = optiga_crypt_create(0, optiga_callback, NULL);

		if ((NULL == p_util) || (NULL == p_crypt))
		{
			break;
		}

		/**
		 * Open the application on OPTIGA which is a precondition to perform any other operations
		 * using optiga_util_open_application
		 */
		CHECK_RESULT(optiga_util_open_application(p_util, restore_from_hibernate));

		// Maximum Power, Minimum Current limitation
		uint8_t current_limit = 15;
		CHECK_RESULT(optiga_util_write_data(p_util,
					 0xE0C4, OPTIGA_UTIL_WRITE_ONLY,
					 0, //offset
					 &current_limit,
					 1));

#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
		example_pair_host_and_optiga_using_pre_shared_secret();
#endif

	}while (FALSE);

	return return_status;
}

static uint16_t optiga_deinit(uint8_t goto_hibernate)
{
	uint16_t return_status = 1;

	do
	{
		/**
		 * Open the application on OPTIGA which is a precondition to perform any other operations
		 * using optiga_util_open_application
		 */
		CHECK_RESULT(optiga_util_close_application(p_util, goto_hibernate));

		//Destroy the instance of optiga_util.
		optiga_util_destroy(p_util);

		//Destroy the instance of optiga_crypt.
		optiga_crypt_destroy(p_crypt);

	}while (FALSE);

	return return_status;
}

/**
 * \brief Generate a SHA256 hash on the message.
 */
uint16_t qi_auth_ptx_crypt_generate_sha256(const uint8_t* p_input, uint16_t inlen, uint8_t* p_digest)
{
	uint16_t  return_status = CRYPT_LIB_ERROR;
	hash_data_from_host_t hash_data_host;
	optiga_hash_context_t hash_ctx;
	uint8_t hash_context_buffer [130];

    do
    {
    	hash_ctx.hash_algo = OPTIGA_HASH_TYPE_SHA_256;
		hash_ctx.context_buffer = hash_context_buffer;
		hash_ctx.context_buffer_length = sizeof(hash_context_buffer);
    	hash_data_host.buffer = p_input;
		hash_data_host.length = inlen;

		CHECK_RESULT(optiga_crypt_hash_start(p_crypt, &hash_ctx));
		CHECK_RESULT(optiga_crypt_hash_update(p_crypt, &hash_ctx, OPTIGA_CRYPT_HOST_DATA, &hash_data_host));
		CHECK_RESULT(optiga_crypt_hash_finalize(p_crypt, &hash_ctx, p_digest));
    }while(FALSE);

	return CRYPT_LIB_OK;
}

uint16_t qi_auth_ptx_crypt_certchain_sha256(uint8_t* p_digest)
{
	uint16_t  return_status = CRYPT_LIB_ERROR;

	optiga_hash_context_t hash_ctx;
    uint8_t hash_context_buffer [130];

    do
    {
		hash_ctx.hash_algo = OPTIGA_HASH_TYPE_SHA_256;
		hash_ctx.context_buffer = hash_context_buffer;
		hash_ctx.context_buffer_length = sizeof(hash_context_buffer);

		hash_data_in_optiga_t optiga_hash = {
				.oid = QI_AUTH_CERTCAIN_OID,
				.offset = 0x03,
				.length = QI_AUTH_CRT_ROOTHASH_LEN + 2 + QI_AUTH_CRT_LEN + QI_AUTH_CRT_LEN  // 32 bytes Root Hash + 121 bytes Interm Cert + 120 bytes Leaf Cert
		};
		CHECK_RESULT(optiga_crypt_hash_start(p_crypt, &hash_ctx));
		CHECK_RESULT(optiga_crypt_hash_update(p_crypt, &hash_ctx, OPTIGA_CRYPT_OID_DATA, &optiga_hash));
		CHECK_RESULT(optiga_crypt_hash_finalize(p_crypt, &hash_ctx, p_digest));

    }while(FALSE);

	return return_status;
}

uint16_t qi_auth_ptx_crypt_sign(uint8_t* p_digest_tbs, uint16_t digest_tbs_size, uint8_t* p_signature, uint16_t* p_signature_len)
{
	uint16_t  return_status = CRYPT_LIB_ERROR;
	// OPTIGA uses DER encoded signatures
	uint8_t der_signature[70];
	uint16_t der_signature_len = sizeof(der_signature);
    do
    {
    	if (NULL == p_signature_len)
    		break;

    	// Sign the resulting digest of nthe TBS Auth Data
    	CHECK_RESULT(optiga_crypt_ecdsa_sign(p_crypt, p_digest_tbs, digest_tbs_size, QI_AUTH_DEVICE_PRIKEY_OID, der_signature, &der_signature_len));

    	asn1_to_ecdsa_rs(der_signature, der_signature_len, p_signature, *p_signature_len);

    }while(FALSE);

    return return_status;
}

uint16_t qi_auth_ptx_crypt_certchain(uint8_t* p_certchain, uint16_t* p_certchain_size)
{
	uint16_t  return_status = CRYPT_LIB_ERROR;
	uint16_t  optiga_oid = QI_AUTH_CERTCAIN_OID;

	do
	{
		/**
		 * 1. Read data from a data object (e.g. certificate data object)
		 *    using optiga_util_read_data.
		 */

		CHECK_RESULT(optiga_util_read_data(p_util, optiga_oid, 0x03, p_certchain, p_certchain_size));
	}while(0);

	return return_status;
}

int32_t qi_auth_ptx_crypt_init(uint8_t restore_from_hibernate)
{
	uint16_t return_status  = CRYPT_LIB_ERROR;

	return_status = optiga_init(restore_from_hibernate);
	if (CRYPT_LIB_OK != return_status)
	{
		printf("Error #1: optiga_init\r\n");
	}

	return return_status;
}

int32_t qi_auth_ptx_crypt_deinit(uint8_t hibernate_chip)
{
	uint16_t return_status  = CRYPT_LIB_ERROR;

	return_status = optiga_deinit(hibernate_chip);
	if (CRYPT_LIB_OK != return_status)
	{
		printf("Error #1: optiga_deinit\r\n");
	}

	return return_status;
}

