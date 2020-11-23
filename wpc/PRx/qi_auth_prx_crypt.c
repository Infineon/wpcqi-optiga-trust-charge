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
* \file qi_auth_prx_crypt.c
*
* \brief   This file implements the APIs for Crypto Wrappers.
* @{
*/

#include <string.h>
#include <ctype.h>
#include <mbedtls/ecp.h>

#include <optiga/optiga_crypt.h>
#include <optiga/optiga_util.h>
#include <optiga/pal/pal_crypt.h>
#include <optiga/pal/pal_os_event.h>
#include <optiga/pal/pal_os_timer.h>
#include <optiga/common/optiga_lib_logger.h>

#include "ecdsa_utils.h"
#include "qi_auth_prx_crypt.h"
#include "qi_auth_fixtures.h"

#define MAXCMD_LEN			255
#define HEXDUMP_COLS    	16

#define QI_AUTH_ROOT_HASH                  qi_auth_cr660_draft5_root_hash
#define QI_AUTH_ROOT_CERT                  qi_auth_cr660_draft5_root_cert
#define QI_AUTH_PTMC_VALUE                 qi_auth_ptmc_value
#define QI_AUTH_REVOKED_RSID               qi_auth_revoked_rsid

#define QI_AUTH_CERTCAIN_OID	           0xE0E0

#define QI_AUTH_DEVICE_PRIKEY_OID          OPTIGA_KEY_ID_E0F0


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
	WAIT_FOR_COMPLETION(return_status);


#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
 // Value of Operational state
 #define LCSO_STATE_CREATION       (0x01)
 // Value of Operational state
 #define LCSO_STATE_OPERATIONAL    (0x07)

 //Currently set to Creation state(defualt value). At the real time/customer side this needs to be LCSO_STATE_OPERATIONAL (0x07)
 #define FINAL_LCSO_STATE          (LCSO_STATE_CREATION)
#endif

static volatile optiga_lib_status_t optiga_lib_status;
static optiga_crypt_t* p_crypt = NULL;
static optiga_util_t* p_util = NULL;


#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
/* Platform Binding Shared Secret (0xE140) Metadata to be updated */

const uint8_t platform_binding_shared_secret_metadata_final [] = {
    //Metadata to be updated
    0x20, 0x17,
        // LcsO
        0xC0, 0x01,
                    FINAL_LCSO_STATE,       // Refer Macro to see the value or some more notes
        // Change/Write Access tag
        0xD0, 0x07,
                    // This allows updating the binding secret during the runtime using shielded connection
                    // If not required to update the secret over the runtime, set this to NEV and
                    // update Metadata length accordingly
                    0xE1, 0xFC, LCSO_STATE_OPERATIONAL,   // LcsO < Operational state
                    0xFE,
                    0x20, 0xE1, 0x40,
        // Read Access tag
        0xD1, 0x03,
                    0xE1, 0xFC, LCSO_STATE_OPERATIONAL,   // LcsO < Operational state
        // Execute Access tag
        0xD3, 0x01,
                    0x00,   // Always
        // Data object Type
        0xE8, 0x01,
                    0x22,   // Platform binding secret type
};
#endif

/// @cond hidden

//#define MBEDTLS_BUFFER_SIZE	8096
//uint8_t mbedtls_memmory_buffer[MBEDTLS_BUFFER_SIZE];


#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
void example_pair_host_and_optiga_using_pre_shared_secret(void);
#endif

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

	return (return_status);
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

	return (return_status);
}


#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION

 /**
  * The below example demonstrates pairing the Host and OPTIGA using a preshared secret for the first time.
  *
  * Note:
  *   1) If the below example is executed once, the LcsO of Platform Binding shared secret is set to Initialization.
  *      The LcsO can't be reverted to previous states.
  *
  *      Please ensure the access conditions and other required settings in the metadata must be accordingly
  *      before setting the LcsO to Initialization state.
  *
  *   2) The metadata gets written in this example is just an example. The user must update this to the respective
  *      needs including LcsO state and access conditions
  *
  * Preconditions: The optiga_util_open_application must be executed before invoking the below example.
  *
  */
 void example_pair_host_and_optiga_using_pre_shared_secret(void)
 {
     uint16_t bytes_to_read;
     uint8_t platform_binding_secret[64];
     uint8_t platform_binding_secret_metadata[44];
     optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
     pal_status_t pal_return_status;
     optiga_util_t * me_util = NULL;
     optiga_crypt_t * me_crypt = NULL;

     OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);

     do
     {
         /**
          * 2. Initialize the protection level and protocol version for the instances
          */
         OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
         OPTIGA_UTIL_SET_COMMS_PROTOCOL_VERSION(me_util,OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);

         OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(me_crypt,OPTIGA_COMMS_NO_PROTECTION);
         OPTIGA_CRYPT_SET_COMMS_PROTOCOL_VERSION(me_crypt,OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);

         /**
          * 3. Read Platform Binding Shared secret (0xE140) data object metadata from OPTIGA
          *    using optiga_util_read_metadata.
          */
         bytes_to_read = sizeof(platform_binding_secret_metadata);
         optiga_lib_status = OPTIGA_LIB_BUSY;
         return_status = optiga_util_read_metadata(me_util,
                                                   0xE140,
                                                   platform_binding_secret_metadata,
                                                   &bytes_to_read);

         WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

         /**
          * 4. Validate LcsO in the metadata.
          *    Skip the rest of the procedure if LcsO is greater than or equal to operational state(0x07)
          */
         if (platform_binding_secret_metadata[4] >= LCSO_STATE_OPERATIONAL)
         {
             // The LcsO is already greater than or equal to operational state
             break;
         }

         /**
          * 5. Generate Random using optiga_crypt_random
          *       - Specify the Random type as TRNG
          *    a. The maximum supported size of secret is 64 bytes.
          *       The minimum recommended is 32 bytes.
          *    b. If the host platform doesn't support random generation,
          *       use OPTIGA to generate the maximum size chosen.
          *       else choose the appropriate length of random to be generated by OPTIGA
          *
          */
         optiga_lib_status = OPTIGA_LIB_BUSY;
         return_status = optiga_crypt_random(me_crypt,
                                             OPTIGA_RNG_TYPE_TRNG,
                                             platform_binding_secret,
                                             sizeof(platform_binding_secret));
         WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

         /**
          * 6. Generate random on Host
          *    If the host platform doesn't support, skip this step
          */


         /**
          * 7. Write random(secret) to OPTIGA platform Binding shared secret data object (0xE140)
          */
         optiga_lib_status = OPTIGA_LIB_BUSY;
         OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
         return_status = optiga_util_write_data(me_util,
                                                0xE140,
                                                OPTIGA_UTIL_ERASE_AND_WRITE,
                                                0,
                                                platform_binding_secret,
                                                sizeof(platform_binding_secret));
         WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

         /**
          * 8. Write/store the random(secret) on the Host platform
          *
          */
         pal_return_status = pal_os_datastore_write(OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID,
                                                    platform_binding_secret,
                                                    sizeof(platform_binding_secret));

         if (PAL_STATUS_SUCCESS != pal_return_status)
         {
             //Storing of Pre-shared secret on Host failed.
             optiga_lib_status = pal_return_status;
             break;
         }



         /**
          * 9. Update metadata of OPTIGA Platform Binding shared secret data object (0xE140)
          */
         optiga_lib_status = OPTIGA_LIB_BUSY;
         OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
         return_status = optiga_util_write_metadata(me_util,
                                                    0xE140,
                                                    platform_binding_shared_secret_metadata_final,
                                                    sizeof(platform_binding_shared_secret_metadata_final));

         WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

         return_status = OPTIGA_LIB_SUCCESS;

     } while(FALSE);
     OPTIGA_EXAMPLE_LOG_STATUS(return_status);
 }

#endif

 /**
 * @}
 */



/**
 * \brief Generate a SHA256 hash on the message.
 */
uint16_t qi_auth_prx_crypt_generate_sha256(const uint8_t* p_input, uint16_t inlen, uint8_t* p_digest)
{
	uint16_t  return_status = CRYPT_LIB_ERROR;
	hash_data_from_host_t hash_data_host;
	optiga_hash_context_t hash_ctx;
	uint8_t hash_context_buffer [130];

    do
    {
    	if (NULL == p_input || 0 == inlen || NULL == p_digest)
    		break;

    	hash_ctx.hash_algo = OPTIGA_HASH_TYPE_SHA_256;
		hash_ctx.context_buffer = hash_context_buffer;
		hash_ctx.context_buffer_length = sizeof(hash_context_buffer);
    	hash_data_host.buffer = p_input;
		hash_data_host.length = inlen;

		CHECK_RESULT(optiga_crypt_hash_start(p_crypt, &hash_ctx));
		CHECK_RESULT(optiga_crypt_hash_update(p_crypt, &hash_ctx, OPTIGA_CRYPT_HOST_DATA, &hash_data_host));
		CHECK_RESULT(optiga_crypt_hash_finalize(p_crypt, &hash_ctx, p_digest));
    }while(FALSE);

	return (CRYPT_LIB_OK);
}

uint16_t qi_auth_prx_crypt_verify_certchain(uint8_t* p_qi_auth_chain_buf, uint16_t qi_auth_chain_buf_size, const uint8_t* p_root_ca, uint16_t root_ca_size)
{
	int return_status  = CRYPT_LIB_ERROR;
#ifdef DRAFT5_SPEC_CERTCHAIN
	uint16_t rsid_counter = 0;
#endif
	mbedtls_x509_crt root_ca;
	mbedtls_x509_crt manufacturer_ca;
	mbedtls_x509_crt product_unit;
	mbedtls_mpi rsid;
	mbedtls_mpi min;
	mbedtls_mpi max;
    uint32_t mbedtls_flags;
    uint8_t p_root_ca_hash[32];

	mbedtls_x509_crt_init( &root_ca );
	mbedtls_x509_crt_init( &manufacturer_ca );
	mbedtls_x509_crt_init( &product_unit );
	mbedtls_mpi_init(&rsid);
	mbedtls_mpi_init(&min);
	mbedtls_mpi_init(&max);

	do
	{
		if (p_qi_auth_chain_buf == NULL || qi_auth_chain_buf_size == 0 || p_root_ca == NULL || root_ca_size == 0)
		{
			return_status = CRYPT_LIB_ERROR_BAD_PARAMETERS;
			break;
		}

		if (CRYPT_LIB_OK != qi_auth_prx_crypt_generate_sha256(p_root_ca, root_ca_size, p_root_ca_hash))
		{
			return_status = CRYPT_LIB_ERROR_HASH_FAILED;
			break;
		}

		// Check whether the provided Root CA Hash is the same as the expected we have stored
		if (0 != memcmp(p_qi_auth_chain_buf, p_root_ca_hash, 32))
		{
			return_status = CRYPT_LIB_ERROR_ROOT_HASH_MISMATCH;
			break;
		}

		// Parse the WPC Root CA Certificate in PEM Format
		return_status = mbedtls_x509_crt_parse( &root_ca, p_root_ca, root_ca_size);
		if( 0 != return_status )
		{
			return_status = CRYPT_LIB_ERROR_PARSE_ROOT_CERT;
			break;
		}

		// Parse the Manufacturer CA Certificate in DER Format (from the chain we've received)
		return_status = mbedtls_x509_crt_parse_der(&manufacturer_ca, p_qi_auth_chain_buf + 32, qi_auth_chain_buf_size - 32);
		if( 0 != return_status )
		{
			return_status = CRYPT_LIB_ERROR_PARSE_MANUF_CERT;
			break;
		}


		// Check that the Manufacturer CA Certificate is signed by the WPC Root CA
		// Optionally you can write here an exact Manufacturer CA PTMC if you like to restrict this
		if( ( return_status = mbedtls_x509_crt_verify( &manufacturer_ca, &root_ca,
		        		                             NULL, NULL /*QI_AUTH_PTMC_VALUE*/, &mbedtls_flags,
		                                             NULL, NULL ) ) != 0 )
		{
			return_status = CRYPT_LIB_ERROR_VERIFY_MANUF_CERT;
			break;
		}

		// Parse the Product Unit Certificate in DER Format (from the chain we've received)
		return_status = mbedtls_x509_crt_parse_der(&product_unit, p_qi_auth_chain_buf + 32 + manufacturer_ca.raw.len,
				qi_auth_chain_buf_size - 32 - + manufacturer_ca.raw.len);
		if( 0 != return_status )
		{
			return_status = CRYPT_LIB_ERROR_PARSE_PRODUCT_CERT;
			break;
		}

		// Check that the Product Unit Certificate Signature is signed by the Manufacturer CA
		if( ( return_status = mbedtls_x509_crt_verify( &product_unit, &manufacturer_ca,
				        		                             NULL, NULL , &mbedtls_flags,
				                                             NULL, NULL ) ) != 0 )
		{
			return_status = CRYPT_LIB_ERROR_VERIFY_PRODUCT_CERT;
			break;
		}

		//Validate Certificates
		// The check below do validate a chain against CR660 introduced options and extensions
#ifdef DRAFT5_SPEC_CERTCHAIN
		if (( 1 != manufacturer_ca.ca_istrue ) || (1 == product_unit.ca_istrue))
		{
			return_status = CRYPT_LIB_ERROR_MANUF_NOTCA;
			break;
		}

		// Check for revoked certificates
		mbedtls_mpi_read_binary(&rsid, product_unit.wpcqi_auth_rsid.p, product_unit.wpcqi_auth_rsid.len);
		for (rsid_counter = 0; rsid_counter < sizeof(QI_AUTH_REVOKED_RSID)/sizeof(QI_AUTH_REVOKED_RSID[0]); rsid_counter ++)
		{
			mbedtls_mpi_read_binary(&min, QI_AUTH_REVOKED_RSID[rsid_counter], 9);
			mbedtls_mpi_read_binary(&max, QI_AUTH_REVOKED_RSID[rsid_counter] + 9, 9);
			// We check here the situation when the RSID provided doesn't lay in the range revoked
			if (((mbedtls_mpi_cmp_abs(&min, &rsid) == -1) || (mbedtls_mpi_cmp_abs(&min, &rsid) == 0)) &&
				((mbedtls_mpi_cmp_abs(&max, &rsid) == 1) || (mbedtls_mpi_cmp_abs(&max, &rsid) == 0)))
			{
				return_status = CRYPT_LIB_ERROR_PRODUCT_CERT_IS_REVOKED;
				break;
			}

		}
#endif

		return_status = 0;

	} while (FALSE);

	mbedtls_mpi_free(&rsid);
	mbedtls_mpi_free(&min);
	mbedtls_mpi_free(&max);
	mbedtls_x509_crt_free(&root_ca);
	mbedtls_x509_crt_free(&manufacturer_ca);
	mbedtls_x509_crt_free(&product_unit);

    return (return_status);
}

uint16_t qi_auth_prx_crypt_get_certchain_info(uint8_t* p_certchain, uint16_t chain_size,
 		                                uint8_t* p_puc_rsid, uint16_t* p_puc_rsid_size,
 									    uint8_t* p_sha256,
 									    uint8_t* p_puc_pubkey, uint16_t* p_puc_pubkey_size)
{
	int return_status  = CRYPT_LIB_ERROR;
	mbedtls_x509_crt manufacturer_ca;
	mbedtls_x509_crt product_unit;
	mbedtls_ecp_keypair* pk;

	do
	{

		if ((p_puc_pubkey == NULL) || (p_puc_rsid == NULL) || (p_puc_rsid_size == NULL) || (p_sha256 == NULL) || (p_puc_pubkey_size == NULL) || (p_puc_pubkey_size == 0))
		{
			return_status = CRYPT_LIB_ERROR_BAD_PARAMETERS;
			break;
		}

		// Parse the Manufacturer CA Certificate in DER Format (from the chain we've received)
		mbedtls_x509_crt_init( &manufacturer_ca );
		return_status = mbedtls_x509_crt_parse_der(&manufacturer_ca, p_certchain + 2 + sizeof(QI_AUTH_ROOT_HASH), chain_size - 2 - sizeof(QI_AUTH_ROOT_HASH));
		if( 0 != return_status )
		{
            return_status = CRYPT_LIB_ERROR_PARSE_MANUF_CERT;
			break;
		}

		// Parse the Product Unit Certificate in DER Format (from the chain we've received)
		mbedtls_x509_crt_init( &product_unit );
		return_status = mbedtls_x509_crt_parse_der(&product_unit, p_certchain + 2 + sizeof(QI_AUTH_ROOT_HASH) + manufacturer_ca.raw.len,
				chain_size - 2 - sizeof(QI_AUTH_ROOT_HASH) - manufacturer_ca.raw.len);
		if( 0 != return_status )
		{
            return_status = CRYPT_LIB_ERROR_PARSE_PRODUCT_CERT;
			break;
		}

		pk = (mbedtls_ecp_keypair *)( product_unit.pk.pk_ctx );
		// If a pubkey is in compressed form, the parser should be able to decompress it
	    if (mbedtls_ecp_point_write_binary( &pk->grp, &pk->Q,
	    		                            MBEDTLS_ECP_PF_UNCOMPRESSED, (size_t *)p_puc_pubkey_size,
	                                        p_puc_pubkey, *p_puc_pubkey_size ) != 0 )
	    {
	    	return_status = CRYPT_LIB_ERROR_EXTRACT_PKEY;
	    	break;
	    }

	    // Copy the RSID
	    memcpy(p_puc_rsid, product_unit.wpcqi_auth_rsid.p, product_unit.wpcqi_auth_rsid.len);
	    *p_puc_rsid_size = product_unit.wpcqi_auth_rsid.len;

	    // Calculate a hash and copy it
	    qi_auth_prx_crypt_generate_sha256(p_certchain, chain_size, p_sha256);

		return_status = 0;

	} while (FALSE);

	mbedtls_x509_crt_free(&manufacturer_ca);
	mbedtls_x509_crt_free(&product_unit);

    return (return_status);
}

uint16_t  qi_auth_prx_crypt_verify_signature(const signature_container_t *p_signature_c)
{
	mbedtls_ecdsa_context ecdsa;
	uint16_t return_status = OPTIGA_CRYPT_ERROR;
	uint8_t p_asn1_signature[72];
	size_t asn1_signature_size = 72;
	uint8_t p_digest[32];
	do
	{
		mbedtls_ecdsa_init(&ecdsa);
		mbedtls_ecp_group_load(&ecdsa.grp, MBEDTLS_ECP_DP_SECP256R1);
		if (mbedtls_ecp_point_read_binary(&ecdsa.grp, &ecdsa.Q, p_signature_c->p_public_key, p_signature_c->public_key_size ) != 0)
		{
			break;
		}

		ecdsa_rs_to_asn1_signature(p_signature_c->p_signature, p_signature_c->p_signature+32, 32,
                                   p_asn1_signature, &asn1_signature_size);

		return_status = qi_auth_prx_crypt_generate_sha256(p_signature_c->p_message, p_signature_c->message_size, p_digest);
		if(CRYPT_LIB_OK != return_status)
		{
			break;
		}

		return_status = mbedtls_ecdsa_read_signature(&ecdsa,
					                                 p_digest, 32,
					                                 p_asn1_signature, asn1_signature_size);


	}while(FALSE);

	return (return_status);
}

int32_t qi_auth_prx_crypt_init(uint8_t restore_from_hibernate)
{
	return (optiga_init(restore_from_hibernate));
}

int32_t qi_auth_prx_crypt_deinit(uint8_t hibernate_chip)
{
	return (optiga_deinit(hibernate_chip));
}


uint16_t  qi_auth_prx_crypt_get_random(uint16_t random_len, uint8_t *p_random, uint16_t random_size)
{
	uint16_t return_status = CRYPT_LIB_ERROR;

    do
    {
        if((NULL == p_random)||(0 == random_size))
        {
            break;
        }

        CHECK_RESULT(optiga_crypt_random(p_crypt, OPTIGA_RNG_TYPE_TRNG, p_random, random_len));

    }while(FALSE);
    return (return_status);
}
