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

#include <optiga/pal/pal_crypt.h>
#include <optiga/pal/pal_os_event.h>
#include <optiga/pal/pal_os_timer.h>
#include <optiga/common/optiga_lib_logger.h>
#include <ecdsa_utils.h>

#include "optiga/comms/optiga_comms.h"

#include "qi_auth_ptx_crypt.h"
#include <ctype.h>

#define MAXCMD_LEN                          255
#define HEXDUMP_COLS                        16

#define QI_AUTH_CERTCAIN_OID                0xE0E0

#define QI_AUTH_DEVICE_PRIKEY_OID           OPTIGA_KEY_ID_E0F0

optiga_comms_t*         p_comms;
static volatile uint8_t optiga_comms_status = OPTIGA_COMMS_BUSY;

/*******************************************************************************
* Function Name: optiga_comms_callback
********************************************************************************
* Summary: A cllaback which is given to the optiga stack worjing in a async mode
*
* Parameters:
*  none
*
* Return:
*  none
*
*******************************************************************************/
static void optiga_comms_callback(void * callback_ctx, optiga_lib_status_t event)
{
    optiga_comms_status = event;
}

/*******************************************************************************
* Function Name: transceive_to_chip
********************************************************************************
* Summary: A function to synchronously transmit/receive data to the chip
*
* Parameters:
*  p_comms A pointer to the comms instance
*  p_tx_data buffer which contains data to be send to the chip
*  tx_data_length data length
*  p_rx_data buffer to store data from the chip
*  p_rx_data_len data length
*
* Return:
*  int
*
*******************************************************************************/
int transceive_to_chip( optiga_comms_t * p_comms,
                        const uint8_t * p_tx_data,
                        uint16_t tx_data_length,
                        uint8_t * p_rx_data,
                        uint16_t * p_rx_data_len)
{
    // transceive data to chip using comms tranceive
    optiga_comms_status = OPTIGA_COMMS_BUSY;
    if(OPTIGA_COMMS_SUCCESS != optiga_comms_transceive(p_comms, p_tx_data, tx_data_length, p_rx_data ,p_rx_data_len))
    {
        return CRYPT_LIB_ERROR;
    }
    //async wait
    while(OPTIGA_COMMS_SUCCESS != optiga_comms_status);

    return 0;
}

/*******************************************************************************
* Function Name: comms_open
********************************************************************************
* Summary: A function to open a communication channel with the optiga
*
* Parameters:
*  pp_comms A double pointer to the comms instance.
*
* Return:
*  int
*
*******************************************************************************/
int comms_open(optiga_comms_t** pp_comms)
{
    // create comms
    *pp_comms = optiga_comms_create(optiga_comms_callback, NULL);
    (*pp_comms)->p_pal_os_event_ctx = (void*)pal_os_event_create(NULL, NULL);
    // perform comms open
    optiga_comms_status = OPTIGA_COMMS_BUSY;
    if(0 != optiga_comms_open(*pp_comms))
    {
        return 0;
    }
    //async wait
    while(0 != optiga_comms_status);
    return 1;

}


/*******************************************************************************
* Function Name: comms_close
********************************************************************************
* Summary: A function to close the communication channel between the endpoints
*
* Parameters:
*  p_comms A pointer to the comms instance.
*
* Return:
*  int
*
*******************************************************************************/
int comms_close( optiga_comms_t * p_comms)
{
    // close comms
    optiga_comms_status = OPTIGA_COMMS_BUSY;
    optiga_comms_close(p_comms);
    while(OPTIGA_COMMS_SUCCESS != optiga_comms_status);

    // destroy the comms
    optiga_comms_destroy(p_comms);
    return 1;
}

static uint16_t optiga_init(uint8_t restore_from_hibernate)
{
    uint8_t open_application_apdu[20] = {
            0x70, 0x00, 0x00, 0x10, 0xD2, 0x76, 0x00, 0x00, 0x04, 0x47, 0x65, 0x6E, 0x41, 0x75, 0x74, 0x68, 0x41, 0x70, 0x70, 0x6C,
    };
    uint16_t apdu_response_length = 20;

    uint8_t ok_response[4] = {0x00, 0x00, 0x00, 0x00};

    // Initialize the hardware
    pal_init();

    // Wait till the chip will wake-up
    pal_os_timer_delay_in_milliseconds(15);

    if(!comms_open(&p_comms))
    {
        return CRYPT_LIB_ERROR;
    }

    transceive_to_chip(p_comms, open_application_apdu, sizeof(open_application_apdu), open_application_apdu, &apdu_response_length);

    if ((apdu_response_length == 4) && (memcmp(open_application_apdu, ok_response, sizeof(ok_response)) == 0))
    {
        return 0;
    }

    return CRYPT_LIB_ERROR;
}

static uint16_t optiga_deinit(uint8_t goto_hibernate)
{
    return CRYPT_LIB_ERROR;
}

static uint16_t get_object_size(uint16_t oid)
{
    // 0x81 - GetDataObject command (SRM)
    // 0x01 - Read metadata
    // 0x00 0x02 - Length
    // Maximum metadata length is 44
    uint8_t      apdu[44] = {0x81, 0x01, 0x00, 0x02};
    uint16_t    apdu_resp_length = 44;
    uint16_t     apdu_length = 0;
    uint16_t    object_size = 0;
    int            j;

    apdu[4] = (uint8_t)(oid >> 8);
    apdu[5] = (uint8_t)(oid & 0x00ff);
    apdu_length = 6;

    transceive_to_chip(p_comms, apdu, apdu_length, apdu, &apdu_resp_length);

    if (apdu_resp_length > 4)
    {
        for (j=0; j < (apdu_resp_length-3); j++)
        {
            if ((apdu[j] == 0xc5) && (apdu[j+1] == 0x02))
            {
                object_size = (apdu[j+2] << 8) + (apdu[j+3] & 0x00ff);
                return object_size;
            }
        }
    }

    return CRYPT_LIB_ERROR;
}

//static uint8_t get_error_code(void)
//{
//    uint8_t      apdu[6] = {0x01, 0x00, 0x00, 0x02, 0xf1, 0xc2};
//    uint16_t     apdu_length = 6;
//
//    transceive_to_chip(p_comms, apdu, apdu_length, apdu, &apdu_length);
//
//    if (apdu_response_length != 0)
//    {
//        return 0;
//    }
//
//    return CRYPT_LIB_ERROR;
//}

static uint16_t calculate_hash_generic(uint8_t internally, uint8_t* p_input, uint16_t inlen, uint8_t* p_digest)
{
    uint8_t  apdu[61]; // We should not have here more than 61 bytes of data including headers
    uint16_t apdu_resp_length = 61;
    uint16_t apdu_length = 0;
    uint16_t object_size = 0;
    uint16_t oid;

    if (p_digest == NULL)
        return CRYPT_LIB_ERROR_BAD_PARAMETERS;

    apdu[0] = 0xB0; // CalcHash command (SRM)
    apdu[1] = 0xE2; // SHA256
    apdu[2] = 0x00; // We should not have here more than 54 bytes of data
    if (!internally)
    {
        apdu[3] = (uint8_t)(inlen + 1 + 2);
        apdu[4] = 0x01; // we do the hash in one round start&final
        apdu[5] = (uint8_t)(inlen >> 8);
        apdu[6] = (uint8_t)(inlen & 0x00ff);
        memcpy(&apdu[7], p_input, inlen);
        apdu_length = 7 + inlen;
    }
    else
    {
        // It should be exactly 2 bytes for the OID
        if (inlen != 2)
            return CRYPT_LIB_ERROR_BAD_PARAMETERS;

        oid = (*p_input << 8) + (*(p_input + 1) & 0x00ff);
        object_size = get_object_size(oid);

        if (object_size == 0)
            return CRYPT_LIB_ERROR;

        apdu[3] = 0x09; // 0x11, 0x0006, OID[2], Offset[2], Length[2]
        apdu[4] = 0x11;
        apdu[5] = 0x00;
        apdu[6] = 0x06;
        apdu[7] = (uint8_t)(*p_input);
        apdu[8] = (uint8_t)(*(p_input + 1));
        apdu[9] = 0x00;
        apdu[10] = 0x03; // The WPC chain has 3 first bytes reserved for internal use
        object_size-=apdu[10]; // we should exclude the offset from the required length
        apdu[11] = (uint8_t)(object_size >> 8);
        apdu[12] = (uint8_t)(object_size & 0x00ff);
        apdu_length = 13;
    }

    transceive_to_chip(p_comms, apdu, apdu_length, apdu, &apdu_resp_length);

    // Sta[1] + UnDef[1] + OutLen[2] + OutData[1 + 1 + 32]
    if (apdu_resp_length == 39)
    {
        memcpy(p_digest, &apdu[4 + 2], 32);
        return 0;
    }

    return CRYPT_LIB_ERROR;
}

/**
 * \brief Generate a SHA256 hash on the message.
 */
uint16_t qi_auth_ptx_crypt_generate_sha256(uint8_t* p_input, uint16_t inlen, uint8_t* p_digest)
{
    return calculate_hash_generic(0, p_input, inlen, p_digest);
}

uint16_t qi_auth_ptx_crypt_certchain_sha256(uint8_t slot, uint8_t* p_digest)
{
    uint16_t oid = 0;

    switch (slot)
    {
        case 0:
            oid = QI_AUTH_CERTCAIN_OID;
            break;
        case 1:
            oid = 0xE0E1;
            break;
        case 2:
            oid = 0xE0E2;
            break;
        case 3:
            oid = 0xE0E3;
            break;
        default:
            return CRYPT_LIB_ERROR_BAD_PARAMETERS;
    }

    if (NULL == p_digest)
    {
        return CRYPT_LIB_ERROR_BAD_PARAMETERS;
    }

    return calculate_hash_generic(1, (uint8_t*)&oid, 2, p_digest);
}

uint16_t qi_auth_ptx_crypt_sign(uint8_t* p_digest_tbs, uint16_t digest_tbs_size, uint8_t* p_signature, uint16_t* p_signature_len)
{
    uint8_t     apdu[82]; // We should not have here more than 59 bytes of data including headers
    uint16_t    apdu_resp_length = 82;
    uint16_t    apdu_length = 0;

    if (NULL == p_signature || NULL == p_signature_len || NULL == p_digest_tbs || 0 == digest_tbs_size || 32 != digest_tbs_size)
        return CRYPT_LIB_ERROR_BAD_PARAMETERS;

    apdu[0] = 0xB1; // CalcSign command (SRM)
    apdu[1] = 0x11; // ECDSA Signature
    apdu[2] = 0x00; // We should not have here more than 35 bytes of data
    apdu[3] = (1 + 2 + 0x20 + 1 + 2 + 2); // tag[1] + Length[2] + Digest[0x20] + tag[1] + Length[2] + OID[2]
    apdu[4] = 0x01;
    apdu[5] = 0x00;
    apdu[6] = 0x20; // The digest should be exactly 32 bytes
    memcpy(&apdu[7], p_digest_tbs, 0x20);
    apdu[7 + 0x20] = 0x03;
    apdu[7 + 0x20 + 1] = 0x00;
    apdu[7 + 0x20 + 2] = 0x02;
    apdu[7 + 0x20 + 3] = (uint8_t)(QI_AUTH_DEVICE_PRIKEY_OID >> 8); // here probably an oid based selection can be implemented
    apdu[7 + 0x20 + 4] = (uint8_t)(QI_AUTH_DEVICE_PRIKEY_OID & 0x00ff);;
    apdu_length = 8 + 0x20 + 4;

    transceive_to_chip(p_comms, apdu, apdu_length, apdu, &apdu_resp_length);

    // Sta[1] + UnDef[1] + OutLen[2] + OutData[signature]
    if (apdu_resp_length > 20)
    {
    	// First four bytes contain the response headers
        asn1_to_ecdsa_rs(&apdu[4], apdu_resp_length-4, p_signature, *p_signature_len);
        return 0;
    }

    return CRYPT_LIB_ERROR;
}

uint16_t qi_auth_ptx_crypt_certchain(uint8_t slot, uint16_t offset, uint8_t* p_certchain, uint16_t* p_certchain_size)
{
    uint16_t    oid;
    uint8_t     apdu[10];
    uint16_t    apdu_length = 0;
    uint16_t    data_length_to_be_read;

    switch (slot)
    {
        case 0:
            oid = QI_AUTH_CERTCAIN_OID;
            break;
        case 1:
            oid = 0xE0E1;
            break;
        case 2:
            oid = 0xE0E2;
            break;
        case 3:
            oid = 0xE0E3;
            break;
        default:
            return CRYPT_LIB_ERROR_BAD_PARAMETERS;
    }

    if (NULL == p_certchain || 0 == *p_certchain_size || offset > *p_certchain_size)
        return CRYPT_LIB_ERROR_BAD_PARAMETERS;

    data_length_to_be_read = *p_certchain_size - offset;

    apdu[0] = 0x81; // GetDataObject command (SRM)
    apdu[1] = 0x00; // Read data
    apdu[2] = 0x00;
    apdu[3] = 0x06;
    apdu[4] = (uint8_t)(oid >> 8);
    apdu[5] = (uint8_t)(oid & 0x00ff);
    apdu[6] = (uint8_t)(offset >> 8);
    apdu[7] = (uint8_t)(offset & 0x00ff) + 3;
    apdu[8] = (uint8_t)(data_length_to_be_read >> 8);
    apdu[9] = (uint8_t)(data_length_to_be_read & 0x00ff);
    apdu_length = 10;

    transceive_to_chip(p_comms, apdu, apdu_length, p_certchain, p_certchain_size);

    // Sta[1] + UnDef[1] + OutLen[2] + OutData[Certificate Chain]
    if (*p_certchain_size > 20)
    {
        return 0;
    }

    return CRYPT_LIB_ERROR;
}

int32_t qi_auth_ptx_crypt_init(uint8_t restore_from_hibernate)
{
    return (optiga_init(restore_from_hibernate));
}

int32_t qi_auth_ptx_crypt_deinit(uint8_t hibernate_chip)
{
    return (optiga_deinit(hibernate_chip));
}

