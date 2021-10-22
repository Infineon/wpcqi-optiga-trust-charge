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
* \file    update_manufacturer_ca.c
*
* \brief   This file implements the APIs, types and data structures for WPC Qi Auth Rel 1.3 implementation
*
*/
#include "optiga/optiga_util.h"
#include "optiga/optiga_crypt.h"
#include "optiga/pal/pal_os_timer.h"


#define OPTIGA_IDENTITY_OFFSET            0
#define OPTIGA_IDENTITY_CHAINSIZE_OFFSET  1
#define OPTIGA_IDENTITY_SIZE              3
#define WPCQI_AUTH_CERTCHAIN_SIZE_OFFSET  OPTIGA_IDENTITY_OFFSET + OPTIGA_IDENTITY_SIZE
#define WPCQI_AUTH_CERTCHAIN_SIZE_SIZE    2
#define WPCQI_AUTH_ROOT_HASH_OFFSET       WPCQI_AUTH_CERTCHAIN_SIZE_OFFSET + WPCQI_AUTH_CERTCHAIN_SIZE_SIZE
#define WPCQI_AUTH_ROOT_HASH_SIZE         32
#define WPCQI_AUTH_MCA_CERT_OFFSET        WPCQI_AUTH_ROOT_HASH_OFFSET + WPCQI_AUTH_ROOT_HASH_SIZE
#define WPCQI_AUTH_MCA_CERT_SIZE_OFFSET   WPCQI_AUTH_MCA_CERT_OFFSET + 2

/**
 * Test Manufacturer CA and Root CA Hash
 */
static const uint8_t test_ifx_test_chain [] = {
    //00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
    0xA5, 0xF0, 0x84, 0xD9, 0x08, 0x76, 0x96, 0x58, 0x65, 0x7D, 0x73, 0x74, 0xBA, 0x82, 0xF5, 0x82,
    0xF0, 0x3A, 0x5B, 0x86, 0x6C, 0xA1, 0x6E, 0xAC, 0x42, 0xA4, 0x37, 0x42, 0xC0, 0x82, 0x22, 0xE3,
    0x30, 0x82, 0x01, 0x0a, 0x30, 0x81, 0xb2, 0x02, 0x04, 0x5e, 0x0a, 0x3c, 0x72, 0x30, 0x0a, 0x06,
    0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30, 0x0e, 0x31, 0x0c, 0x30, 0x0a, 0x06,
    0x03, 0x55, 0x04, 0x03, 0x0c, 0x03, 0x57, 0x50, 0x43, 0x30, 0x20, 0x17, 0x0d, 0x32, 0x30, 0x30,
    0x37, 0x30, 0x33, 0x31, 0x32, 0x30, 0x36, 0x35, 0x39, 0x5a, 0x18, 0x0f, 0x39, 0x39, 0x39, 0x39,
    0x31, 0x32, 0x33, 0x31, 0x32, 0x33, 0x35, 0x39, 0x35, 0x39, 0x5a, 0x30, 0x11, 0x31, 0x0f, 0x30,
    0x0d, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x06, 0x4d, 0x3a, 0x43, 0x41, 0x43, 0x41, 0x30, 0x59,
    0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a, 0x86, 0x48,
    0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x6e, 0x95, 0x15, 0xd8, 0x68, 0xbd, 0x1f,
    0x37, 0x4d, 0x50, 0x43, 0x75, 0x31, 0x06, 0xe8, 0xe2, 0x78, 0xb0, 0x2f, 0x9e, 0xff, 0x1f, 0x16,
    0x7b, 0xbf, 0x71, 0x40, 0xc1, 0xe0, 0x1f, 0x00, 0x1c, 0x38, 0x4c, 0xe2, 0x29, 0xae, 0x53, 0xab,
    0x0b, 0x5f, 0x49, 0xa7, 0xf5, 0x0e, 0x19, 0x26, 0x96, 0xca, 0x98, 0x0c, 0x13, 0x77, 0x70, 0xed,
    0x70, 0x41, 0x54, 0xd2, 0x23, 0x63, 0xaa, 0xc6, 0x46, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48,
    0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x47, 0x00, 0x30, 0x44, 0x02, 0x20, 0x43, 0x02, 0x1b, 0x7c,
    0x1c, 0x95, 0x59, 0xe9, 0x47, 0x9b, 0xad, 0x90, 0x72, 0xf9, 0x99, 0xea, 0x79, 0x5a, 0xc5, 0x59,
    0xbb, 0x32, 0xf5, 0x01, 0x54, 0xec, 0xd5, 0x47, 0xc6, 0x90, 0x7f, 0xd0, 0x02, 0x20, 0x5a, 0x36,
    0xfb, 0x1d, 0x8c, 0xa6, 0x1f, 0xa2, 0x65, 0x6b, 0xc4, 0x3b, 0x55, 0x31, 0xef, 0xdf, 0xa5, 0x10,
    0xcc, 0xa7, 0x7c, 0x2a, 0xb5, 0x3f, 0x95, 0x66, 0x0a, 0x35, 0x2b, 0xc3, 0xb8, 0x9d
};

/**
 * Incorrect Manufacturer CA
 */
static const uint8_t nok_ifx_manufacturer_ca [] = {
    //00    01    02    03    04    05    06    07    08    09    0A    0B    0C    0D    0E    0F
    0x30, 0x82, 0x01, 0x0f, 0x30, 0x81, 0xb7, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x04, 0x12, 0x34,
    0xab, 0xcd, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x30, 0x0e,
    0x31, 0x0c, 0x30, 0x0a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x03, 0x57, 0x50, 0x43, 0x30, 0x20,
    0x17, 0x0d, 0x37, 0x30, 0x30, 0x31, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5a, 0x18,
    0x0f, 0x39, 0x39, 0x39, 0x39, 0x31, 0x32, 0x33, 0x31, 0x32, 0x33, 0x35, 0x39, 0x35, 0x39, 0x5a,
    0x30, 0x11, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x06, 0x4d, 0x3a, 0x78,
    0x78, 0x78, 0x78, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01,
    0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x4f, 0xb1,
    0x8d, 0xde, 0x80, 0xed, 0x4e, 0x4c, 0x78, 0x43, 0x09, 0xc8, 0xb2, 0x75, 0xca, 0x56, 0xdf, 0x7e,
    0x49, 0x76, 0x10, 0x1c, 0xfc, 0x5e, 0x2d, 0xae, 0xa1, 0x62, 0xf1, 0x93, 0x73, 0xf0, 0x7f, 0x57,
    0x3c, 0xa5, 0xfd, 0x33, 0xa2, 0x0d, 0x98, 0xee, 0x99, 0xb8, 0x7d, 0x83, 0xfd, 0xe9, 0xba, 0x81,
    0x36, 0x0a, 0xa1, 0xc4, 0x64, 0x36, 0x0e, 0x5d, 0x68, 0x85, 0x5b, 0x28, 0xff, 0x2a, 0x30, 0x0a,
    0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03, 0x47, 0x00, 0x30, 0x44, 0x02,
    0x20, 0x1d, 0xf7, 0x70, 0x4c, 0x36, 0x08, 0x93, 0x76, 0x98, 0x83, 0x01, 0x89, 0x98, 0x70, 0x7f,
    0x1d, 0x59, 0xf6, 0x71, 0xbc, 0x6c, 0x01, 0x73, 0xbd, 0x6e, 0xfa, 0x6b, 0x19, 0xdd, 0xcd, 0x74,
    0x40, 0x02, 0x20, 0x38, 0x32, 0x12, 0xd9, 0x82, 0xa2, 0xf1, 0x03, 0x62, 0x59, 0xd5, 0x41, 0x1a,
    0xa1, 0x63, 0x3e, 0xc4, 0x62, 0x7b, 0xf5, 0x49, 0x8c, 0xef, 0x5e, 0xe4, 0x4f, 0xe5, 0x83, 0x7e,
    0xf6, 0x6b, 0x17
};

/**
 * metadata to unlock write option for the certificate object 0xE0E0
 */
static const uint8_t unlock_write_metadata [] = {
    //Metadata tag in the data object
    0x20, 0x05,
        //Change tag in the metadata
        0xD0, 0x03,
            //LcsO < Operation
            0xE1 , 0xFC, 0x07,
};

/**
 * metadata to lock write option for the certificate object 0xE0E0
 */
static const uint8_t lock_write_metadata [] = {
    //Metadata tag in the data object
    0x20, 0x03,
        //Change tag in the metadata
        0xD0, 0x01,
            //Never
            0xFF
};


/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
static optiga_util_t * p_util = NULL;
//lint --e{818} suppress "argument "context" is not used in the sample provided"
static void optiga_util_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

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
            break; \
    }


#ifdef UPDATE_CERTCHAIN
/* If your eval kit was affected by the problem of the wrong Length provisioning
 * during the update_certificate_chain() call this function might help you to 
 * restore the correct length. It gets the certificate chain length and update it
 * on the chip
 */
void update_certificate_chain_length(void)
{
	optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    uint8_t certchain[800];
    uint16_t certchain_size = 800;
    uint8_t new_certchain_size[2];

	do
	{
		/**
		 * 1. Create OPTIGA Util Instance.
		 */
		p_util = optiga_util_create(0, optiga_util_callback, NULL);
		if (NULL == p_util)
		{
			break;
		}

		/*
		 * Step 1. Extract the certificate chain and get it's size
		 */
		optiga_lib_status = OPTIGA_LIB_BUSY;
		// Step 1.1 read only the length of the Manufacturer CA certificate. Length is part of the certificate, and located in the 3rd and 4th bytes
		return_status = optiga_util_read_data(p_util, 0xE0E0, 0x00, certchain, &certchain_size);
		WAIT_FOR_COMPLETION(return_status);

		// Minus three bytes as they are occupied by the C6 tag and an internal infor about the length.
		certchain_size -= 3;
		new_certchain_size[0] = (uint8_t)((certchain_size >> 8) & 0xff);
		new_certchain_size[1] = (uint8_t)(certchain_size & 0xff);

		/**
		 * Step 2. Unlock the certificate object 0xE0E0 using optiga_util_write_metadata.
		 */
		optiga_lib_status = OPTIGA_LIB_BUSY;
		return_status = optiga_util_write_metadata(p_util, 0xE0E0, unlock_write_metadata, sizeof(unlock_write_metadata));
		WAIT_FOR_COMPLETION(return_status);

		/**
		 * Step 3. write a new certificate chain back
		 */
		optiga_lib_status = OPTIGA_LIB_BUSY;
		return_status = optiga_util_write_data(p_util, 0xE0E0, OPTIGA_UTIL_WRITE_ONLY, 3, new_certchain_size, 2);
		WAIT_FOR_COMPLETION(return_status);

		/**
		 * Step 4. Lock back the certificate object 0xE0E0 using optiga_util_write_metadata.
		 */
		optiga_lib_status = OPTIGA_LIB_BUSY;
		return_status = optiga_util_write_metadata(p_util, 0xE0E0, lock_write_metadata, sizeof(lock_write_metadata));
		WAIT_FOR_COMPLETION(return_status);

		return_status = OPTIGA_LIB_SUCCESS;

	} while (FALSE);

	if (p_util)
	{
		//Destroy the instance after the completion of usecase if not required.
		optiga_util_destroy(p_util);
	}
}
#endif


void update_certificate_chain(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    uint8_t mca_cert[400];
    uint8_t mca_asn1_certsize[2];
    uint16_t mca_asn1_certsize_size = 2;
    uint16_t mca_cert_size = 400;
    // 3 bytes OPTIGA Tags + 2 bytes length + 32 bytes hash + 4 bytes ASN.1 Tag (2 last bytes is length)
    uint16_t mca_cert_offset = OPTIGA_IDENTITY_SIZE + WPCQI_AUTH_CERTCHAIN_SIZE_SIZE + WPCQI_AUTH_ROOT_HASH_SIZE;
    uint8_t pu_cert[400];
    uint16_t pu_cert_size = 400;
    uint8_t new_certchain[800];
    uint16_t new_certchain_size = 800;

    do
    {
        /**
         * 1. Create OPTIGA Util Instance.
         */
        p_util = optiga_util_create(0, optiga_util_callback, NULL);
        if (NULL == p_util)
        {
            break;
        }

        /*
         * Step 1. Read out the length and the Manufacturer CA Certificate into the internal buffer
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        // Step 1.1 read only the length of the Manufacturer CA certificate. Length is part of the certificate, and located in the 3rd and 4th bytes
        return_status = optiga_util_read_data(p_util, 0xE0E0, mca_cert_offset + 2, mca_asn1_certsize, &mca_asn1_certsize_size);
        WAIT_FOR_COMPLETION(return_status);
        // 4 is for the first 4 bytes of the certificate (which includes an ASN.1 Sequence Tag and a length)
        mca_cert_size = ((mca_asn1_certsize[0] << 8) + mca_asn1_certsize[1]) + 4;

        optiga_lib_status = OPTIGA_LIB_BUSY;
        // Step 1.2. Read the Manufacturer CA certificate based on the retried length
        return_status = optiga_util_read_data(p_util, 0xE0E0, mca_cert_offset, mca_cert, &mca_cert_size);
        WAIT_FOR_COMPLETION(return_status);


        optiga_lib_status = OPTIGA_LIB_BUSY;
        /*
         * Step 2. Read out the the Product Unit certificate.
         * We will read till end of the chain, as there is nothing after the certificate and optiga won't read beyond this
         */
        return_status = optiga_util_read_data(p_util, 0xE0E0, mca_cert_offset + mca_cert_size, pu_cert, &pu_cert_size);
        WAIT_FOR_COMPLETION(return_status);

        /*
         * Step 3. Compare the Manufacturer CA Certificate with the one we want to substitute
         */
        if (sizeof(nok_ifx_manufacturer_ca) < mca_cert_size)
        {
            if (memcmp(mca_cert, nok_ifx_manufacturer_ca, sizeof(nok_ifx_manufacturer_ca)) != 0)
            {
                // it's not the certificate which should be replaced, return from the function
                break;
            }
        }
        else {
            if (memcmp(mca_cert, nok_ifx_manufacturer_ca, mca_cert_size) != 0)
            {
                // it's not the certificate which should be replaced, return from the function
                break;
            }
        }

        /*
         * Step 4. We need now to substitute the certificate in the optiga with the one we expect
         */
        new_certchain[0] = 0xc6;
        new_certchain[OPTIGA_IDENTITY_CHAINSIZE_OFFSET] = (uint8_t)(((sizeof(test_ifx_test_chain) + pu_cert_size) >> 8) & 0xff);
        new_certchain[OPTIGA_IDENTITY_CHAINSIZE_OFFSET + 1] = (uint8_t)((sizeof(test_ifx_test_chain) + pu_cert_size) & 0xff);
        new_certchain[WPCQI_AUTH_CERTCHAIN_SIZE_OFFSET] = new_certchain[1];
        new_certchain[WPCQI_AUTH_CERTCHAIN_SIZE_OFFSET + 1] = new_certchain[2];
        new_certchain_size = 5;
        // we offset some bytes. 1 byte for the C6 tag, 4 bytes for the doubled length of the chain
        memcpy(new_certchain + 5, test_ifx_test_chain, sizeof(test_ifx_test_chain));
        new_certchain_size += sizeof(test_ifx_test_chain);
        memcpy(new_certchain + 5 + sizeof(test_ifx_test_chain), pu_cert, pu_cert_size);
        new_certchain_size += pu_cert_size;

        /**
         * Step 5. Unlock the certificate object 0xE0E0 using optiga_util_write_metadata.
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_write_metadata(p_util, 0xE0E0, unlock_write_metadata, sizeof(unlock_write_metadata));
        WAIT_FOR_COMPLETION(return_status);

        /**
         * Step 6. write a new certificate chain back
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_write_data(p_util, 0xE0E0, OPTIGA_UTIL_ERASE_AND_WRITE, 0, new_certchain, new_certchain_size);
        WAIT_FOR_COMPLETION(return_status);

        /**
         * Step 7. Lock back the certificate object 0xE0E0 using optiga_util_write_metadata.
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_write_metadata(p_util, 0xE0E0, lock_write_metadata, sizeof(lock_write_metadata));
        WAIT_FOR_COMPLETION(return_status);

        return_status = OPTIGA_LIB_SUCCESS;

    } while (FALSE);

    if (p_util)
    {
        //Destroy the instance after the completion of usecase if not required.
        optiga_util_destroy(p_util);
    }
}


/**
* @}
*/
