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
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/sha256.h>

#include "ecdsa_utils.h"
#include "qi_auth_prx_crypt.h"
#include "qi_auth_fixtures.h"

#define MAXCMD_LEN			255
#define HEXDUMP_COLS    	16

#define QI_AUTH_ROOT_HASH                  qi_auth_cr660_draft5_root_hash
#define QI_AUTH_ROOT_CERT                  qi_auth_cr660_draft5_root_cert
#define QI_AUTH_PTMC_VALUE                 qi_auth_ptmc_value
#define QI_AUTH_REVOKED_RSID               qi_auth_revoked_rsid

//#define MBEDTLS_BUFFER_SIZE	8096
//uint8_t mbedtls_memmory_buffer[MBEDTLS_BUFFER_SIZE];

static mbedtls_ctr_drbg_context ctx;

/**
 * \brief Generate a SHA256 hash on the message.
 */
uint16_t qi_auth_prx_crypt_generate_sha256(const uint8_t* p_input, uint16_t inlen, uint8_t* p_digest)
{
	uint16_t  return_status = CRYPT_LIB_ERROR;

    do
    {
    	if (NULL == p_input || 0 == inlen || NULL == p_digest)
    		break;

    	mbedtls_sha256_ret(p_input, inlen, p_digest, 0);

    	return_status = CRYPT_LIB_OK;

    }while(0);


	return (return_status);
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
		return_status = mbedtls_x509_crt_parse_der(&manufacturer_ca, p_qi_auth_chain_buf + WPCQI_AUTH_CRT_ROOTHASH_LEN,
				                                   qi_auth_chain_buf_size - WPCQI_AUTH_CRT_ROOTHASH_LEN);
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
		return_status = mbedtls_x509_crt_parse_der(&product_unit, p_qi_auth_chain_buf + WPCQI_AUTH_CRT_ROOTHASH_LEN + manufacturer_ca.raw.len,
				qi_auth_chain_buf_size - WPCQI_AUTH_CRT_ROOTHASH_LEN - manufacturer_ca.raw.len);
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

	} while (0);

	mbedtls_mpi_free(&rsid);
	mbedtls_mpi_free(&min);
	mbedtls_mpi_free(&max);
	mbedtls_x509_crt_free(&root_ca);
	mbedtls_x509_crt_free(&manufacturer_ca);
	mbedtls_x509_crt_free(&product_unit);

    return return_status;
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

	} while (0);

	mbedtls_x509_crt_free(&manufacturer_ca);
	mbedtls_x509_crt_free(&product_unit);

    return return_status;
}

uint16_t  qi_auth_prx_crypt_verify_signature(const signature_container_t *p_signature_c)
{
	mbedtls_ecdsa_context ecdsa;
	uint16_t return_status = CRYPT_LIB_ERROR;
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


	}while(0);

	return return_status;
}

int32_t qi_auth_prx_crypt_init(uint8_t restore_from_hibernate)
{
    mbedtls_ctr_drbg_init(&ctx);

	return 0;
}

int32_t qi_auth_prx_crypt_deinit(uint8_t hibernate_chip)
{
    mbedtls_ctr_drbg_free(&ctx);

	return 0;
}

int mbedtls_hardware_poll( void *data,
                           unsigned char *output, size_t len, size_t *olen )
{
    int error = 0;

    /* A good source of entropy should be provided here */
    for (int i=0; i < len; i++)
    {
    	output[i] |= *(output + i - 0x40);
    }

    return error;
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

        mbedtls_ctr_drbg_random(&ctx, p_random, random_size);

        return_status = CRYPT_LIB_OK;

    }while(0);

    return return_status;
}
