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
* \file qi_auth_crypt.c
*
* \brief   This file implements the APIs for Crypto Wrappers.
* @{
*/

#include <string.h>
#include <mbedtls/ecp.h>

#include <optiga/optiga_crypt.h>
#include <optiga/optiga_util.h>
#include <optiga/pal/pal_crypt.h>
#include <optiga/pal/pal_os_event.h>
#include <optiga/pal/pal_os_timer.h>
#include <optiga/common/optiga_lib_logger.h>

#include "ecdsa_utils.h"
#include "qi_auth_crypt.h"
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
optiga_crypt_t* p_crypt = NULL;
optiga_util_t* p_util = NULL;

/// @cond hidden

//#define MBEDTLS_BUFFER_SIZE	8096
//uint8_t mbedtls_memmory_buffer[MBEDTLS_BUFFER_SIZE];

uint8_t qi_auth_rel13_root_cert[] = {
		0x01, 0x01, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x57, 0x50, 0x43, 0x00,
		0x00, 0x00, 0x57, 0x50, 0x43, 0x00, 0x00, 0x00, 0x03, 0x2c, 0xea, 0x09, 0x11, 0x6e, 0x60, 0x43,
		0x34, 0x79, 0x5d, 0xc9, 0x5c, 0x5c, 0xf0, 0xd5, 0xcf, 0xc1, 0x4c, 0x2c, 0x83, 0xc2, 0xbb, 0x6a,
		0xae, 0xd9, 0xc2, 0x5a, 0x6e, 0x3c, 0x4f, 0x68, 0x97, 0x02, 0x17, 0xe6, 0x17, 0xf0, 0xb6, 0x44,
		0x39, 0x28, 0x27, 0x8f, 0x96, 0x99, 0x9e, 0x69, 0xa2, 0x3a, 0x4f, 0x2c, 0x15, 0x2b, 0xdf, 0x6d,
		0x6c, 0xdf, 0x66, 0xe5, 0xb8, 0x02, 0x82, 0xd4, 0xed, 0x4c, 0x2f, 0x57, 0xc2, 0x3c, 0x4e, 0x3c,
		0x6d, 0x5d, 0xd6, 0x7e, 0xc8, 0x02, 0xda, 0x43, 0x18, 0xf4, 0x87, 0x98, 0xc6, 0x02, 0x3d, 0xef,
		0xe6, 0xcf, 0x51, 0x22, 0xc0, 0xf7, 0x37, 0x20, 0x12
};

//uint8_t qi_auth_plugfest_root_cert[] = {
//		0x01, 0x01, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x57, 0x50, 0x43, 0x00,
//		0x00, 0x00, 0x57, 0x50, 0x43, 0x00, 0x00, 0x00, 0x03, 0x29, 0x9c, 0xbb, 0x09, 0xc0, 0x06, 0x94,
//		0x6b, 0x05, 0x09, 0x57, 0xb7, 0x8c, 0x57, 0xbe, 0x4e, 0xf8, 0x23, 0x56, 0xd7, 0xb1, 0x8c, 0xbf,
//		0xc7, 0x2f, 0xfa, 0xec, 0x1c, 0x43, 0xe5, 0x8e, 0x54, 0x81, 0x7e, 0x10, 0x4b, 0xb2, 0xb5, 0x9f,
//		0xbb, 0x28, 0xa8, 0xb9, 0x86, 0xda, 0xb8, 0x6f, 0x21, 0x3f, 0xc9, 0xfd, 0xc1, 0x65, 0xdd, 0xec,
//		0xa2, 0xe8, 0xd0, 0xb6, 0xa9, 0x01, 0xf9, 0x94, 0xcf, 0xee, 0x6e, 0x36, 0xa3, 0xdf, 0x65, 0xfc,
//		0x9f, 0xd5, 0xbb, 0x87, 0x51, 0xcb, 0x52, 0x83, 0x72, 0xca, 0xbf, 0x3a, 0xfc, 0xc8, 0x88, 0x73,
//		0x61, 0x45, 0x18, 0x1b, 0xd3, 0xdd, 0x58, 0xbd, 0x13
//};

uint8_t qi_auth_rel13_root_hash[] = {
		0xBB, 0xEC, 0x6A, 0xE8, 0x0D, 0x77, 0x64, 0xE3, 0x8F, 0x22, 0xF5, 0x32, 0xF3, 0xE1, 0x43, 0x20,
		0xC6, 0x1F, 0xE2, 0x81, 0x2A, 0xD6, 0xA9, 0x34, 0x9C, 0x30, 0xE2, 0x9E, 0x13, 0x03, 0xFD, 0xF5
};
//uint8_t qi_auth_plugfest_root_hash[] = {
//		0xa6, 0x1f, 0x3a, 0x79, 0x81, 0xf1, 0xd0, 0xb6, 0x64, 0xf8, 0x93, 0x5c, 0xd9, 0x98, 0xf4, 0xf6,
//		0x4d, 0x57, 0x3d, 0xa4, 0xcc, 0x58, 0x46, 0xf6, 0xaf, 0x01, 0x2f, 0xeb, 0xda, 0x1c, 0x8a, 0xe1
//};

extern void example_pair_host_and_optiga_using_pre_shared_secret(void);

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static void optiga_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
}


static uint16_t optiga_init(void)
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
		CHECK_RESULT(optiga_util_open_application(p_util, 0));

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

static int mbedtls_ecp_decompress( const mbedtls_ecp_group *grp, const unsigned char *input, size_t ilen,
								   unsigned char *output, size_t *olen, size_t osize )
{
    int ret;
    size_t plen;
    mbedtls_mpi r;
    mbedtls_mpi x;
    mbedtls_mpi n;

    plen = mbedtls_mpi_size( &grp->P );

    *olen = 2 * plen + 1;

    if( osize < *olen )
        return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );

    if( ilen != plen + 1 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if( input[0] != 0x02 && input[0] != 0x03 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    // output will consist of 0x04|X|Y
    memcpy( output, input, ilen );
    output[0] = 0x04;

    mbedtls_mpi_init( &r );
    mbedtls_mpi_init( &x );
    mbedtls_mpi_init( &n );

    // x <= input
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &x, input + 1, plen ) );

    // r = x^2
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &r, &x, &x ) );

    // r = x^2 + a
    // AY: r = r - 3
    if( grp->A.p == NULL ) {
        // Special case where a is -3
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &r, &r, 3 ) );
    } else {
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &r, &r, &grp->A ) );
    }

    // r = x^3 + ax
    // AY: r = r * x
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &r, &r, &x ) );

    // r = x^3 + ax + b
    // AY: r = r * b
    /*
     * static const mbedtls_mpi_uint secp256r1_b[] = {
		BYTES_TO_T_UINT_8( 0x4B, 0x60, 0xD2, 0x27, 0x3E, 0x3C, 0xCE, 0x3B ),
		BYTES_TO_T_UINT_8( 0xF6, 0xB0, 0x53, 0xCC, 0xB0, 0x06, 0x1D, 0x65 ),
		BYTES_TO_T_UINT_8( 0xBC, 0x86, 0x98, 0x76, 0x55, 0xBD, 0xEB, 0xB3 ),
		BYTES_TO_T_UINT_8( 0xE7, 0x93, 0x3A, 0xAA, 0xD8, 0x35, 0xC6, 0x5A ),
	};
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &r, &r, &grp->B ) );

    // Calculate quare root of r over finite field P:
    //   r = sqrt(x^3 + ax + b) = (x^3 + ax + b) ^ ((P + 1) / 4) (mod P)

    // n = P + 1
    // AY: n = p + 1
    /*
     * static const mbedtls_mpi_uint secp256r1_p[] = {
		BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
		BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 ),
		BYTES_TO_T_UINT_8( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ),
		BYTES_TO_T_UINT_8( 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF ),
	};
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_int( &n, &grp->P, 1 ) );

    // n = (P + 1) / 4
    // AY: n = n / 2
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &n, 2 ) );

    // r ^ ((P + 1) / 4) (mod p)
    // Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
    // mbedtls_mpi *X, const mbedtls_mpi *A, const mbedtls_mpi *E, const mbedtls_mpi *N, mbedtls_mpi *_RR
    // AY: r = (r ^ n) mod p
    MBEDTLS_MPI_CHK( mbedtls_mpi_exp_mod( &r, &r, &n, &grp->P, NULL ) );

    // Select solution that has the correct "sign" (equals odd/even solution in finite group)
    // (input == 3): 0 == r[0] & 0x01
    //                  -> True: r = p - r
    // (input == 2): 1 ==  r[0] & 0x01
    //				    -> True: r = p - r
    if( (input[0] == 0x03) != mbedtls_mpi_get_bit( &r, 0 ) ) {
        // r = p - r
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &r, &grp->P, &r ) );
    }

    // y => output
    ret = mbedtls_mpi_write_binary( &r, output + 1 + plen, plen );

cleanup:
    mbedtls_mpi_free( &r );
    mbedtls_mpi_free( &x );
    mbedtls_mpi_free( &n );

    return( ret );
}


/**
 * \brief Verifies the ECC signature using the given public key.
 */
static uint16_t qi_auth_crypt_verify_ecc_signature(const uint8_t* p_pubkey, uint8_t pubkey_size,
		                                      const uint8_t* p_data, uint16_t data_size,
											  const uint8_t* p_sign, uint8_t sign_size)
{
	uint16_t return_status = (int32_t)CRYPT_LIB_VERIFY_SIGN_FAIL;
    uint8_t p_digest[32];
    uint8_t decompr_pubkey[70];
    uint8_t asn1_signature[71];
    size_t asn1_signature_len = 71;
    size_t pubkey_len = 65;
    uint8_t* p_pk = NULL;

    mbedtls_ecp_group grp;
    // Public Key
    mbedtls_ecp_point Q;
    mbedtls_mpi r;
    mbedtls_mpi s;

    mbedtls_ecp_point_init( &Q );
    mbedtls_mpi_init( &r );
    mbedtls_mpi_init( &s );
    mbedtls_ecp_group_init( &grp );

    mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SECP256R1);

    do
    {
        if((NULL == p_pubkey)||(NULL == p_data)||(NULL == p_sign))
        {
            break;
        }

        if (p_pubkey[0] != 0x04) {
            mbedtls_ecp_decompress(&grp, p_pubkey, pubkey_size, decompr_pubkey + 3, &pubkey_len, 65);
            p_pk = decompr_pubkey;
            p_pk[0] = 0x03;
            p_pk[1] = 0x42;
            p_pk[2] = 0x00;
            pubkey_len += 3;
        } else {
        	// WPC specifies so far only compressed public keys
        	break;
        }

        return_status = qi_auth_crypt_generate_sha256(p_data, data_size, p_digest);
        if(CRYPT_LIB_OK != return_status)
        {
            break;
        }

        ecdsa_rs_to_asn1_integers(p_sign, p_sign + 32, 32, asn1_signature, &asn1_signature_len);

        public_key_from_host_t pkey_details = {
        		                               p_pk,
                                               pubkey_len,
                                               (uint8_t)OPTIGA_ECC_CURVE_NIST_P_256
                                               };
        CHECK_RESULT(optiga_crypt_ecdsa_verify (p_crypt,
        										p_digest,
												32,
												asn1_signature,
												asn1_signature_len,
												OPTIGA_CRYPT_HOST_DATA,
												&pkey_details))
    }while(FALSE);

    return return_status;
}

/**
*
* Verifies and validates the given To Be Verified (tbv) certificate<br>
*
* \param[in]  PpsCaCert              pointer to CA Certificate
* \param[in]  PpsOPTIGACert          pointer to Security Chip Certificate
*
* \retval    #INT_LIB_OK
* \retval    #INT_LIB_ERROR
* \retval    #INT_LIB_NULL_PARAM
*
*/
static uint16_t _validate_qi_auth_certificate(certificate_container_t *p_cert, certificate_container_t *p_tbv_cert)
{
	uint16_t return_status = CRYPT_LIB_ERROR;
    signature_container_t sign_vector ;
    uint8_t prefix[] = {0x43};
    uint8_t manuf_code_bin[2] = {0x00, 0x00};
    uint8_t root_manuf_code_bin[2] = {0xff, 0xff};
    uint16_t parsed_manuf_code = 0;
    uint8_t data_to_sign[60];
    do
    {
        if((NULL == p_cert)||(NULL == p_cert->p_cert_data)||
            (NULL == p_cert->p_cert_sign)||
            (NULL == p_cert->p_pub_key)||
            (NULL == p_tbv_cert)||(NULL == p_tbv_cert->p_cert_data)||
            (NULL == p_tbv_cert->p_cert_sign)||
            (NULL == p_tbv_cert->p_pub_key))
        {
            break;
        }

        memcpy(data_to_sign, prefix, sizeof(prefix));
        if ((p_tbv_cert->subj_id[0] == 0x4d) && (p_tbv_cert->subj_id[1] == 0x3a))
        {
        	parsed_manuf_code = strtol((char *)&p_tbv_cert->subj_id[2], NULL, 16);
        	manuf_code_bin[0] = (uint8_t)(parsed_manuf_code >> 8);
        	manuf_code_bin[1] = (uint8_t)(parsed_manuf_code & 0x00ff);

        }else if (memcmp(p_tbv_cert->subj_id,"WPC\0", 4) == 0)
        {
        	memcpy(manuf_code_bin, root_manuf_code_bin , sizeof(root_manuf_code_bin));
        } else if ((p_tbv_cert->issuer_id[0] == 0x4d) && (p_tbv_cert->issuer_id[1] == 0x3a))
        {
        	parsed_manuf_code = strtol((char *)&p_tbv_cert->issuer_id[2], NULL, 16);
        	manuf_code_bin[0] = (uint8_t)(parsed_manuf_code >> 8);
        	manuf_code_bin[1] = (uint8_t)(parsed_manuf_code & 0x00ff);
        }

        memcpy(data_to_sign + sizeof(prefix), manuf_code_bin, sizeof(manuf_code_bin));
        memcpy(data_to_sign + sizeof(prefix) + sizeof(manuf_code_bin), p_tbv_cert->p_cert_data, p_tbv_cert->cert_data_size);

        //verify signature of the Security Chip certificate
        sign_vector.p_message = data_to_sign;
        sign_vector.message_size = p_tbv_cert->cert_data_size + sizeof(manuf_code_bin) + sizeof(prefix);
        sign_vector.p_signature = p_tbv_cert->p_cert_sign;
        sign_vector.signature_size = p_tbv_cert->cert_sign_size;
        sign_vector.p_public_key = p_cert->p_pub_key;
        sign_vector.public_key_size = p_cert->pub_key_size;
        return_status = qi_auth_crypt_verify_signature(&sign_vector);
    }while(FALSE);

    return return_status;
}

/**
 * \brief Generate a SHA256 hash on the message.
 */
uint16_t qi_auth_crypt_generate_sha256(const uint8_t* p_input, uint16_t inlen, uint8_t* p_digest)
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

uint16_t qi_auth_crypt_certchain_sha256(uint8_t* p_digest)
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

uint16_t qi_auth_crypt_sign(uint8_t* p_digest_tbs, uint16_t digest_tbs_size, uint8_t* p_signature, uint16_t* p_signature_len)
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

uint16_t qi_auth_crypt_certchain(uint8_t* p_certchain, uint16_t* p_certchain_size)
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

uint16_t qi_auth_crypt_verify_certchain(uint8_t* p_qi_auth_chain_buf, uint16_t qi_auth_chain_buf_size,
		                               uint8_t* p_pubkey, uint8_t pubkey_size)
{
	uint16_t return_status  = CRYPT_LIB_ERROR;
	uint8_t p_root_pubkey[LENGTH_PUB_KEY];
	uint8_t p_interm_pubkey[LENGTH_PUB_KEY];
	uint8_t p_leaf_pubkey[LENGTH_PUB_KEY];
	certificate_container_t qi_auth_chain[3];

	do
	{
		//Parse WPC Chain of Certificates
		qi_auth_chain[0].p_pub_key = p_root_pubkey;
		qi_auth_chain[0].pub_key_size = LENGTH_PUB_KEY;
		qi_auth_chain[1].p_pub_key  = p_interm_pubkey;
		qi_auth_chain[1].pub_key_size = LENGTH_PUB_KEY;
		qi_auth_chain[2].p_pub_key  = p_leaf_pubkey;
		qi_auth_chain[2].pub_key_size = LENGTH_PUB_KEY;
		return_status = qi_auth_crypt_parse_certchain(p_qi_auth_chain_buf, qi_auth_chain_buf_size, qi_auth_chain, 3);
		if(CRYPT_LIB_OK != return_status)
		{
			break;
		}

		//Validate Leaf Certificate
		//mbedtls_memory_buffer_alloc_init(mbedtls_memmory_buffer, MBEDTLS_BUFFER_SIZE);
		return_status = _validate_qi_auth_certificate(&qi_auth_chain[1], &qi_auth_chain[2]);
		if(CRYPT_LIB_OK != return_status)
		{
			break;
		}

		//Validate Intermediate Certificate
		//mbedtls_memory_buffer_alloc_init(mbedtls_memmory_buffer, MBEDTLS_BUFFER_SIZE);
		return_status = _validate_qi_auth_certificate(&qi_auth_chain[0], &qi_auth_chain[1]);
		if(CRYPT_LIB_OK != return_status)
		{
			break;
		}

		memcpy(p_pubkey, p_leaf_pubkey, LENGTH_PUB_KEY);

	} while (FALSE);

    return return_status;
}


uint16_t  qi_auth_crypt_verify_signature(const signature_container_t *p_signature_c)
{
	uint16_t return_status  = CRYPT_LIB_ERROR;
    do
    {
        if((NULL == p_signature_c)||
            (NULL == p_signature_c->p_message)||
            (NULL == p_signature_c->p_signature)||
            (NULL == p_signature_c->p_public_key))
        {
            break;
        }
        //check if length is equal to zero
        if((ZERO_LENGTH == p_signature_c->message_size)||
            (ZERO_LENGTH == p_signature_c->signature_size)||
            (ZERO_LENGTH == p_signature_c->public_key_size))
        {
        	return_status = (int32_t)CRYPT_LIB_LENZERO_ERROR;
            break;
        }
        //mbedtls_memory_buffer_alloc_init(mbedtls_memmory_buffer, MBEDTLS_BUFFER_SIZE);
        return_status = qi_auth_crypt_verify_ecc_signature(p_signature_c->p_public_key, p_signature_c->public_key_size,
                                                  p_signature_c->p_message, p_signature_c->message_size,
                                                  p_signature_c->p_signature, p_signature_c->signature_size);
    }while(FALSE);
    return return_status;
}

int32_t qi_auth_crypt_init(void)
{
	uint16_t return_status  = CRYPT_LIB_ERROR;

	return_status = optiga_init();
	if (CRYPT_LIB_OK != return_status)
	{
		printf("Error #1: optiga_init\r\n");
	}

	return return_status;
}


uint16_t  qi_auth_crypt_get_random(uint16_t random_len, uint8_t *p_random, uint16_t random_size)
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
    return return_status;
}


uint16_t  qi_auth_crypt_parse_certchain(uint8_t* p_chain, uint16_t chain_size, certificate_container_t *p_crt, uint8_t max_chain_size)
{

    //lint --e{818} suppress "PpsCertificate is out parameter"
	uint16_t 	status  = (int32_t)CRYPT_LIB_ERROR;
    uint8_t* 	dev_root_hash = NULL;
    uint16_t	parsed_off = 0;
    uint8_t		cur_crt = 0;

    do
    {
        if((NULL == p_chain) || (NULL == p_crt))
        {
            break;
        }
        //Check for length equal to zero
        if((ZERO_LENGTH == chain_size) || (QI_AUTH_MINCHAIN_LEN > chain_size) ||
           (QI_AUTH_MINCHAIN > max_chain_size))
        {
            status = CRYPT_LIB_LENZERO_ERROR;
            break;
        }

        dev_root_hash = &p_chain[parsed_off];

        if (0 != memcmp(dev_root_hash, qi_auth_rel13_root_hash, QI_AUTH_CRT_ROOTHASH_LEN)) {
            status = CRYPT_LIB_ERROR;
            break;
        }

        memcpy(p_crt[cur_crt].p_pub_key, qi_auth_rel13_root_cert + QI_AUTH_CRT_PUBKEY_OFF, QI_AUTH_CRT_PUBKEY_LEN);
        p_crt[cur_crt].pub_key_size = QI_AUTH_CRT_PUBKEY_LEN;

        p_crt[cur_crt].p_cert_data = qi_auth_rel13_root_cert;
        // ToDo This should be changed to the sign offset byte read
        p_crt[cur_crt].cert_data_size = QI_AUTH_CRT_SIGN_OFF;

        p_crt[cur_crt].p_cert_sign= qi_auth_rel13_root_cert + QI_AUTH_CRT_SIGN_OFF;
        p_crt[cur_crt].cert_sign_size = QI_AUTH_CRT_SIGN_LEN;

        memcpy(p_crt[cur_crt].issuer_id, qi_auth_rel13_root_cert + QI_AUTH_CRT_ISSUERID_OFF, QI_AUTH_CRT_ISSUERID_LEN);

        memcpy(p_crt[cur_crt].subj_id, qi_auth_rel13_root_cert + QI_AUTH_CRT_SUBJID_OFF, QI_AUTH_CRT_SUBJID_LEN);

        parsed_off += QI_AUTH_CRT_ROOTHASH_LEN;
        cur_crt++;

        while (parsed_off < chain_size)
        {
        	if ((NULL == p_crt[cur_crt].p_pub_key) || (ZERO_LENGTH == p_crt[cur_crt].pub_key_size)) {
        		break;
        	}
			memcpy(p_crt[cur_crt].p_pub_key, &p_chain[parsed_off] + QI_AUTH_CRT_PUBKEY_OFF, QI_AUTH_CRT_PUBKEY_LEN);
			p_crt[cur_crt].pub_key_size = QI_AUTH_CRT_PUBKEY_LEN;

			p_crt[cur_crt].p_cert_data = &p_chain[parsed_off];
			// ToDo This should be changed to the sign offset byte read
			p_crt[cur_crt].cert_data_size = QI_AUTH_CRT_SIGN_OFF;

			p_crt[cur_crt].p_cert_sign = &p_chain[parsed_off] + QI_AUTH_CRT_SIGN_OFF;
			p_crt[cur_crt].cert_sign_size = QI_AUTH_CRT_SIGN_LEN;

	        memcpy(p_crt[cur_crt].issuer_id, &p_chain[parsed_off] + QI_AUTH_CRT_ISSUERID_OFF, QI_AUTH_CRT_ISSUERID_LEN);

	        memcpy(p_crt[cur_crt].subj_id, &p_chain[parsed_off] + QI_AUTH_CRT_SUBJID_OFF, QI_AUTH_CRT_SUBJID_LEN);

			cur_crt++;
			parsed_off += QI_AUTH_CRT_LEN;
        }


        status =   CRYPT_LIB_OK;
    }while(FALSE);

    return status;
}
