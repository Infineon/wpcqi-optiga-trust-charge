/**
 * @file usbd.c
 * @date 2016-02-10
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * USBD v4.0.14 - The USB core driver for XMC4000 family of controllers. It does the USB protocol handling.
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the           
 * following conditions are met:   
 *                                                                              
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following   
 *   disclaimer.                        
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the    
 *   following disclaimer in the documentation and/or other materials provided with the distribution.     
 *                         
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 *   products derived from this software without specific prior written permission. 
 *                                             
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,  
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE   
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,  
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR        
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,      
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE   
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes       
 * with Infineon Technologies AG (dave@infineon.com).        
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-16:
 *     - Initial version.      
 * 2015-03-18:
 *     - Guidelines updated.
 * 2015-04-23:
 *     - Updated the review comments from AEs.
 * 2015-06-20:
 *     - Updated the file header.
 * 2016-02-10:
 *     - Removed the macro CLOCK_XMC4_USBCLK_ENABLED check and added the error message in UI.
 *     
 * @endcond 
 *
 */

/***********************************************************************************************************************
 * HEADER FILES                                                                                                      
 **********************************************************************************************************************/
#include <stdlib.h>
#include <xmc_scu.h>
#include "./usb/core/events.h"
#include "./usb/core/usb_task.h"
#include "usbd.h"

/**********************************************************************************************************************
 * MACROS                                                                                                            
 **********************************************************************************************************************/

/**
 * USB setup packet size -for 3 packets
 **/
#define USBD_EP0_3SETUP_PKT_SIZE				(24U)

/**
 * USB EP0 setup packet size
 **/
#define USBD_EP0_SETUP_PKT_SIZE					(8U)

/**
 * USB EP0 buffer size
 **/
#define USBD_EP0_BUF_SIZE						    (256U)

/**
 * USB EP0 maximum packet size
 **/
#define USBD_EP0_MAX_PKT_SIZE					  (64U)

/**
 * Used to mask the byte value
 **/
#define USBD_BYTE_MASK							    (0xFFU)

#define USBD_WORD_SIZE                  (2U)

#define USBD_MAX_NUM_EPS                (7U)


/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/
 
 /***********************************************************************************************************************
 * GLOBAL DATA
 **********************************************************************************************************************/
/*Global variable defining the USB device*/
USB_Device_t device;

/*endpoint point 0 buffer allocation*/
uint8_t endpoint0_in_buffer[USBD_EP0_BUF_SIZE];
uint8_t endpoint0_out_buffer[USBD_EP0_BUF_SIZE];


/***********************************************************************************************************************
 * LOCAL ROUTINES 
 **********************************************************************************************************************/

/**
 * Function to enable USB interrupt
 **/
static void USB_EnableUSBInterrupt(void);

/**
 * Function to disable USB interrupt
 **/
static void USB_DisableUSBInterrupt(void);

/**
 * Function to handle the EP0 stall
 **/
static void USBD_HandleEP0_Stall(void);

/**
 * The function to handle the device request routed from the USB LLD
 **/
static void USBD_Handle_DeviceRequest(void);

/**
 * The function to handle the Control IN EP interrupt
 **/
static void USBD_HandleEP0_IN(void);

/**
 * The function to handle the Control OUT EP interrupt
 **/
static void USBD_HandleEP0_OUT(void);

/**
 * The function to handle the setup packet
 **/
static void USBD_HandleEP0_SETUP(void);

/**
 * \brief Device Event Handler for the device controller driver
 *
 * Handles device events send by the device controller driver(XMC library).
 **/
static void USBD_SignalDeviceEventHandler(XMC_USBD_EVENT_t event);

/**
 * \brief Endpoint Event Handler for the device controller driver
 *
 * Handles endpoint events send by the device controller driver(XMC library).
 **/
static void USBD_SignalEndpointEvent_Handler(uint8_t ep_addr, XMC_USBD_EP_EVENT_t ep_event);

/**********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/

/*
 * Initialize driver core and driver
 *
 * Intializes the USB driver core data structures and sets it into default
 * state. Afterwards it initializes the USB device controller driver and prepare
 *  it for connection via USBD_Connect.
 */
USBD_STATUS_t USBD_Init(USBD_t *handle)
{
	USBD_STATUS_t status = USBD_STATUS_SUCCESS;

	XMC_ASSERT("USBD_Init: invalid handle", (handle != NULL));


	/* Disable USB to resolve the disconnect issue on self powered devices.
	 * Later in the XMC_USBD_Init() it enables the USB.
	 */
	XMC_USBD_Disable();

	memset(&device,0x0,sizeof(USB_Device_t));
	USB_DeviceState = (uint8_t)DEVICE_STATE_Unattached;
	device.Driver = &Driver_USBD0;
	device.ep0_state = USBD_EP0_STATE_IDLE;
	device.Endpoints[0].InBuffer = endpoint0_in_buffer;
	device.Endpoints[0].InBufferLength = USBD_EP0_BUF_SIZE;
	device.Endpoints[0].OutBuffer = endpoint0_out_buffer;
	device.Endpoints[0].OutBufferLength = USBD_EP0_BUF_SIZE;
	device.Endpoints[0].Direction = (uint8_t)0;
	device.Endpoints[0].IsConfigured = (uint8_t)1;
	device.Endpoints[0].IsEnabled = (uint8_t)1;
	device.Endpoints[0].MaxPacketSize = (uint8_t)USBD_EP0_MAX_PKT_SIZE;

	if ((handle->event_cb->control_request == 0) ||
	    (handle->event_cb->get_descriptor == 0) ||
	    (handle->event_cb->config_changed == 0))
	{
	  status = USBD_STATUS_FAILURE;
	}
	else
	{
	  device.events = handle->event_cb;

	  XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_USB);

	  /* First initalize the device */
	  handle->usb_init.cb_xmc_device_event = USBD_SignalDeviceEventHandler;
	  handle->usb_init.cb_endpoint_event = USBD_SignalEndpointEvent_Handler;

	  status = (USBD_STATUS_t)device.Driver->Initialize(&handle->usb_init);
	  if (USBD_STATUS_SUCCESS == status)
	  {
	  /* Enable Interrupts in NVIC */
	  USB_EnableUSBInterrupt();

	  /* then configure endpoint 0 */
	  status = (USBD_STATUS_t)device.Driver->EndpointConfigure((uint8_t)0,XMC_USBD_ENDPOINT_TYPE_CONTROL,
	       (uint16_t)XMC_USBD_MAX_PACKET_SIZE);
	  }

	  if (USBD_STATUS_SUCCESS != status)
	  {
	  XMC_USBD_Disable();
	  if (USBD_STATUS_SUCCESS == (USBD_STATUS_t)device.Driver->Uninitialize())
	  {
	    USB_DisableUSBInterrupt();
	  }
	  }
	}


	return status;
}

/*
 * Connects the USB device to the bus.
 *
 * Tell the USB device controller driver to connect to the bus. Successful connection will be shown
 * through update of the device status ( USB_DeviceState) and a firing of the USB device connect event.
 */
USBD_STATUS_t USBD_Connect(void)
{
  return (USBD_STATUS_t)device.Driver->DeviceConnect();
}

/*
 * Disconnect the device from the bus.
 *
 * Tell the USB device controller driver to disconnect from the bus. Successful
 * disconnection will be shown through update of the device status
 * (USB_DeviceState) and a firing of the USB device disconnect event.
 */
USBD_STATUS_t USBD_Disconnect(void)
{
  return (USBD_STATUS_t)device.Driver->DeviceDisconnect();
}

/*
 * Is the enumeration finished?
 *
 * Check with the USB device controller driver if the enumeration is done.
 * Returns 1 on completion of enumeration.
 */
uint32_t USBD_IsEnumDone(void)
{
  return device.Driver->IsEnumDone();
}


/*
 * Set the buffer for an endpoint
 *
 * The user has to ensure that an endpoint has a valid buffer for proper data transfer.
 *
 */
void USBD_SetEndpointBuffer(uint8_t addr, uint8_t *buf, uint16_t len)
{
  uint32_t number;
  XMC_ASSERT("USBD_SetEndpointBuffer: invalid buffer", (buf != NULL));

  number = (uint32_t)addr & (uint32_t)ENDPOINT_EPNUM_MASK;
  XMC_ASSERT("USBD_SetEndpointBuffer: invalid ep address", (number < (uint8_t)USBD_MAX_NUM_EPS));

  if (addr & (uint8_t)ENDPOINT_DIR_MASK)
  {
    device.Endpoints[number].InBuffer = buf;
    device.Endpoints[number].InBufferLength = len;
  }
  else
  {
    device.Endpoints[number].OutBuffer = buf;
    device.Endpoints[number].OutBufferLength = len;
  }

}

/*
 * Entry point for exception vector table.
 *
 * This function gets called, if a USB exception (interrupt) was thrown and
 * dispatches it to XMC_USBD_IRQHandler.
 */
void USB0_0_IRQHandler(void)
{
  XMC_USBD_IRQHandler(&(USBD_handle->usb_init));
}

/*
 * API to retrieve the version of the USBD APP
 */
DAVE_APP_VERSION_t USBD_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = USBD_MAJOR_VERSION;
  version.minor = USBD_MINOR_VERSION;
  version.patch = USBD_PATCH_VERSION;

  return version;
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/

/**
 * Enables and sets the priority of USB Interrupt
 *
 * First the interrupt priority is set and then the interrupt is enabled in the NVIC.
 */
static void USB_EnableUSBInterrupt(void)
{
  NVIC_SetPriority(USB0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), USB_PRE_EMPTION_PRIORITY, 
  USB_SUB_PRIORITY));
  NVIC_ClearPendingIRQ(USB0_0_IRQn);
  NVIC_EnableIRQ(USB0_0_IRQn);
}

/**
 * Disables the USB interrupt in the NVIC.
 *
 * Before the interrupt gets disabled, it will clear the pending IRQs.
 */
static void USB_DisableUSBInterrupt(void)
{
  NVIC_ClearPendingIRQ(USB0_0_IRQn);
  NVIC_DisableIRQ(USB0_0_IRQn);
}


/**
 * Handle protocol stall on EP0
 *
 * Stalls EP0 and then restarts a new transfer including setting EP0 state to
 * 																\ref USBD_EP0_STATE_IDLE.
 */
static void USBD_HandleEP0_Stall(void)
{
  /* When we stall ep0 as protocol stall, we go back into idle state and
  * start a new read */
  if (USBD_STATUS_SUCCESS == (USBD_STATUS_t)device.Driver->EndpointStall(ENDPOINT_DIR_IN | 0U,1U))
  {
    device.ep0_state = USBD_EP0_STATE_IDLE;
    if ( USBD_STATUS_SUCCESS != (USBD_STATUS_t)device.Driver->EndpointReadStart(0U,USBD_EP0_3SETUP_PKT_SIZE))
    {
      XMC_ASSERT("USBD_HandleEP0_Stall: EndpointReadStart failed", 0);
    }
  }
}

/**
 * Handle device request on endpoint 0
 *
 * By default any USB device has to support a subset of device control request
 * defined by the USB specification. All request required by the spec are
 * handle within this function.Before handling of any request it dispatches it
 * to the \ref control_request event callback function. In this callback, user can
 * handle some custom request or override the request handling in this function.
 * If the user has handled the request, he has to call \ref Endpoint_ClearSETUP.
 *
 */
static void USBD_Handle_DeviceRequest(void)
{
  uint32_t length = 0U;
  uint32_t ret;
  void *buffer = NULL;
  uint32_t status = 0U;
  uint32_t index;
  uint32_t value;
  uint32_t tmp_value;
  uint32_t tmp_index;
  

  value = (uint32_t)USB_ControlRequest.wValue & (uint32_t)USBD_BYTE_MASK;

  /* Handling of class/vendor requests */
  if (NULL != device.events->control_request)
  {
    device.events->control_request();
  }

  if (1U == device.IsSetupRecieved)
  {
    /* default request handling */
    switch (USB_ControlRequest.bRequest)
    {
      case REQ_ClearFeature:
        if ((uint8_t)REQREC_ENDPOINT == (USB_ControlRequest.bmRequestType & (uint8_t)REQ_RECIPIENT_MASK))
        {
          index = (uint32_t)USB_ControlRequest.wIndex & (uint32_t)ENDPOINT_EPNUM_MASK;
          if ((((uint8_t)DEVICE_STATE_Configured == USB_DeviceState) || ((uint16_t)0U == USB_ControlRequest.wIndex)) &&
                  (1U == device.Endpoints[index].IsConfigured))
          {
            device.Endpoints[index].IsHalted = 0U;
            if (USBD_STATUS_SUCCESS != 
                                 (USBD_STATUS_t)device.Driver->EndpointStall((uint8_t)USB_ControlRequest.wIndex,false))
            {
              XMC_ASSERT("USBD_Handle_DeviceRequest: EndpointStall failed", 0);
            }
          }
          else
          {
            USBD_HandleEP0_Stall();
          }
          break;
        }
        if (REQREC_DEVICE == ((uint32_t)USB_ControlRequest.bmRequestType & REQ_RECIPIENT_MASK))
        {
          device.remote_wakeup = 0U;
          break;
        }
        USBD_HandleEP0_Stall();
        break;

      case REQ_GetConfiguration:
        if (0U == (uint32_t)device.Driver->EndpointWrite(0U,&device.configuration,1U))
        {
          XMC_ASSERT("USBD_Handle_DeviceRequest: EndpointWrite failed on get configuration request", 0);
        }
        break;

      case REQ_GetDescriptor:
        if (NULL != device.events->get_descriptor)
        {
          tmp_value = (uint32_t)USB_ControlRequest.wValue;
          tmp_index = (uint32_t)USB_ControlRequest.wIndex;
          length = (uint32_t)device.events->get_descriptor((uint16_t)tmp_value, (uint16_t)tmp_index, (void*)&buffer);
        }

        if (0U == length)
        {
          USBD_HandleEP0_Stall();
        }
        else
        {
          if (length >= (uint32_t)USB_ControlRequest.wLength)
          {
            length = (uint32_t)USB_ControlRequest.wLength;
          }

          ret = (uint32_t)device.Driver->EndpointWrite(0U,buffer,length);
          device.Endpoints[0].InDataLeft = length - ret;
          device.Endpoints[0].InDataBuffer = (uint8_t *)buffer + ret;
        }
        break;

      case REQ_GetInterface:
        if ((uint8_t)DEVICE_STATE_Configured == USB_DeviceState)
        {          
          if (0U == (uint32_t)device.Driver->EndpointWrite(0U, 
             &device.interface_settings[USB_ControlRequest.wIndex], 1U))
          {
            XMC_ASSERT("USBD_Handle_DeviceRequest: EndpointWrite failed on REQ_GetInterface", 0);
          }
          break;
        }
        if ((uint8_t)DEVICE_STATE_Addressed == USB_DeviceState)
        {
          USBD_HandleEP0_Stall();
          break;
        }
        break;

      case REQ_GetStatus:
        if (REQREC_DEVICE == ((uint32_t)USB_ControlRequest.bmRequestType & REQ_RECIPIENT_MASK))
        {
          status = (uint32_t)((uint8_t)(device.remote_wakeup << (uint8_t)1) | device.self_powered);					
          if (0U == (uint32_t)device.Driver->EndpointWrite(0U, (uint8_t*)&status, USBD_WORD_SIZE))
          {
            XMC_ASSERT("USBD_Handle_DeviceRequest: EndpointWrite failed on REQ_GetStatus", 0);
          }          
          break;
        }
        if ((REQREC_INTERFACE == ((uint32_t)USB_ControlRequest.bmRequestType & REQ_RECIPIENT_MASK)) &&
            ((uint8_t)DEVICE_STATE_Configured == USB_DeviceState))
        {
          status = 0U;
          if (0U == (uint32_t)device.Driver->EndpointWrite(0U, (uint8_t*)&status, USBD_WORD_SIZE))
          {
            XMC_ASSERT("USBD_Handle_DeviceRequest: EndpointWrite failed on REQ_GetStatus- REQREC_INTERFACE", 0);
          }          
          break;
        }

        if (REQREC_ENDPOINT == ((uint32_t)USB_ControlRequest.bmRequestType & REQ_RECIPIENT_MASK))
        {
          index = (uint32_t)(USB_ControlRequest.wIndex & USBD_BYTE_MASK & ENDPOINT_EPNUM_MASK);
          if ((((uint8_t)DEVICE_STATE_Configured == USB_DeviceState) || ((uint16_t)0 == USB_ControlRequest.wIndex)) &&
              (1U == device.Endpoints[index].IsConfigured))
          {
            status = (uint32_t)device.Endpoints[index].IsHalted;
            if (0U == (uint32_t)device.Driver->EndpointWrite(0U, (uint8_t*)&status, USBD_WORD_SIZE))
            {
              XMC_ASSERT("USBD_Handle_DeviceRequest: EndpointWrite failed on REQ_GetStatus- REQREC_ENDPOINT", 0);
            }              
            break;
          }
        }
        /* default stall */
        USBD_HandleEP0_Stall();
        break;

      case REQ_SetAddress:
        if (0U == value)
        {
          USB_DeviceState = (uint8_t)DEVICE_STATE_Default;
        }
        else
        {
          USB_DeviceState = (uint8_t)DEVICE_STATE_Addressed;
        }
        if (USBD_STATUS_SUCCESS == (USBD_STATUS_t)device.Driver->DeviceSetAddress((uint8_t)value,
                                   XMC_USBD_SET_ADDRESS_STAGE_SETUP))
        {
          if (NULL != device.events->set_address)
          {
            device.events->set_address();
          }
        }
        break;

      case REQ_SetConfiguration:
        /* Regardless the state update the configuration to unconfigure
         * endpoints */
        device.configuration = (uint8_t)value;
        if (NULL != device.events->config_changed)
        {
          device.events->config_changed();
        }

        /* when config 0 is choosen, we are back in address state */
        if (0U == value)
        {
          USB_DeviceState = (uint8_t)DEVICE_STATE_Addressed;
          break;
        }
        /* go ahead only with vailid config. (must be set in event) */
        if (1U == device.IsConfigured)
        {
          USB_DeviceState = (uint8_t)DEVICE_STATE_Configured;
        }
        else
        {
          USBD_HandleEP0_Stall();
        }
        break;

      case REQ_SetDescriptor:
        /* Set Descriptor not supported, so stall */
        USBD_HandleEP0_Stall();
        break;

      case REQ_SetInterface:
        if ((uint8_t)DEVICE_STATE_Configured == USB_DeviceState)
        {
          tmp_value = (uint32_t)USB_ControlRequest.wValue;
          tmp_index = (uint32_t)USB_ControlRequest.wIndex;
          device.interface_settings[tmp_index] =	(uint8_t)tmp_value;
          break;
        }
        if ((uint8_t)DEVICE_STATE_Addressed == USB_DeviceState)
        {
          USBD_HandleEP0_Stall();
          break;
        }
        break;

      case REQ_SetFeature:
        /* we do not support test mode */
        if ((uint32_t)FEATURE_SEL_TestMode == value)
        {
          USBD_HandleEP0_Stall();
          break;
        }
        /* configured state */
        if ((uint8_t)DEVICE_STATE_Configured == USB_DeviceState)
        {
          switch (value)
          {
            case FEATURE_SEL_DeviceRemoteWakeup:
              device.remote_wakeup = 1U;
              break;
            case FEATURE_SEL_EndpointHalt:
              index = ((uint32_t)USB_ControlRequest.wIndex & (uint32_t)USBD_BYTE_MASK & 
                      (uint32_t)XMC_USBD_ENDPOINT_NUMBER_MASK);
              if (0U == device.Endpoints[index].IsConfigured)
              {
                USBD_HandleEP0_Stall();
              }
              else
              {
                device.Endpoints[index].IsHalted = 1U;
                if (USBD_STATUS_SUCCESS != 
                                  (USBD_STATUS_t)device.Driver->EndpointStall((uint8_t)USB_ControlRequest.wIndex,true))
                {
                  XMC_ASSERT("USBD_Handle_DeviceRequest: EndpointStall failed- FEATURE_SEL_EndpointHalt", 0);
                }                
              }
              break;
            default:
              break;
          }
          break;
        }
        /* when addressed, only ep0 can be halted */
        if ((uint8_t)DEVICE_STATE_Addressed == USB_DeviceState)
        {
          if (((uint32_t)FEATURE_SEL_EndpointHalt == value) &&
              (REQREC_ENDPOINT == ((uint32_t)USB_ControlRequest.bmRequestType & REQ_RECIPIENT_MASK)) &&
              ((uint16_t)0 == ((uint16_t)USB_ControlRequest.wIndex & (uint16_t)USBD_BYTE_MASK)))
          {
            device.Endpoints[0].IsHalted = 1U;
            USBD_HandleEP0_Stall();
            break;
          }
        }
        /* default behaviour is stall */
        USBD_HandleEP0_Stall();
        break;

      case REQ_SynchFrame:
        /* Not yet supported */
        USBD_HandleEP0_Stall();
        break;

      default:
        USBD_HandleEP0_Stall();
        break;
    }
  }
  device.IsSetupRecieved = 0U;
}

/**
 * Handle complete IN transfer on EP0
 *
 * In USBD_EP0_STATE_IN_DATA state it starts a receive and switches to \ref USBD_EP0_STATE_OUT_STATUS
 * state.
 * In USBD_EP0_STATE_IN_STATUS state it starts a new read of setup packets and switches
 * to USBD_EP0_STATE_IDLE.
 */
static void USBD_HandleEP0_IN()
{
  if (USBD_EP0_STATE_IN_DATA == device.ep0_state)
  {
    /* Read zero length out data packet */
    device.Driver->EndpointReadStart((uint8_t)0,0U);
    device.ep0_state = USBD_EP0_STATE_OUT_STATUS;
  }
  else if (USBD_EP0_STATE_IN_STATUS == device.ep0_state)
  {
    /* Request new setup packet */
    device.Driver->EndpointReadStart((uint8_t)device.Endpoints[0].Address,(uint32_t)USBD_EP0_3SETUP_PKT_SIZE);
    device.ep0_state = USBD_EP0_STATE_IDLE;
  }
  else
  {
    /*Assert*/
    XMC_ASSERT("USBD_HandleEP0_IN: invalid ep0 state", 0);
  }
}


/**
 * Handle complete OUT transfer on EP0.
 *
 * Handles the OUT packet based on the state of endpoint 0. Starts a new read
 * for new SETUP packets, when in \ref USBD_EP0_STATE_OUT_STATUS. When endpoint 0 is in
 * USBD_EP0_STATE_OUT_DATA state, it handles the received data and starts a write
 * transaction for \ref USBD_EP0_STATE_IN_STATUS.
 */
static void USBD_HandleEP0_OUT()
{
  if (USBD_EP0_STATE_OUT_DATA == device.ep0_state)
  {
    /* Now we have the data for handling the request */
    USBD_Handle_DeviceRequest();
    /* Zero length packet for status stage */
    device.Driver->EndpointWrite((uint8_t)0,(uint8_t*)0,(uint32_t)0);
    device.ep0_state = USBD_EP0_STATE_IN_STATUS;
  }
  else if (USBD_EP0_STATE_OUT_STATUS == device.ep0_state)
  {
    /* Request new setup packet */
    device.Driver->EndpointReadStart((uint8_t)device.Endpoints[0].Address,(uint32_t)USBD_EP0_3SETUP_PKT_SIZE);
    device.ep0_state = USBD_EP0_STATE_IDLE;
  }
  else
  {
    /*Assert*/
    XMC_ASSERT("USBD_HandleEP0_OUT: invalid ep0 state", 0);
  }
}

/**
 * Handle SETUP packet on EP0.
 *
 * Handles the setup packet and switches to correct state. If data is send from host to device it switches
 * into USBD_EP0_STATE_OUT_DATA state. When the host sends all data within the setup packet and when wLength
 * equals zero, it starts processing the request and sends in status response including the switch to
 * USBD_EP0_STATE_IN_STATUS. When the host expects data from the device, the function processes the control
 * request and switches to USBD_EP0_STATE_IN_DATA state.
 */
static void USBD_HandleEP0_SETUP()
{
  /* read setup packet from ep0 */
  uint32_t ret_val;

  ret_val = (uint32_t)device.Driver->EndpointRead((uint8_t)0,(void*)&USB_ControlRequest,
            (uint32_t)USBD_EP0_SETUP_PKT_SIZE);
  device.IsSetupRecieved = (uint8_t)true;

  if ((uint32_t)USBD_EP0_SETUP_PKT_SIZE == ret_val)
  {
    /* if length is zero we have only a in_status phase */
    if (0U == (uint32_t)USB_ControlRequest.wLength)
    {
      device.ep0_state = USBD_EP0_STATE_IN_STATUS;
      USBD_Handle_DeviceRequest();
      device.Driver->EndpointWrite((uint8_t)0, (uint8_t*)0, (uint32_t)0);
    }
    else
    {
      if (USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_DIRECTION)
      {
        device.ep0_state = USBD_EP0_STATE_IN_DATA;
        USBD_Handle_DeviceRequest();
      }
      else
      {
        device.ep0_state = USBD_EP0_STATE_OUT_DATA;
        /* Do not process request here, first read data */
        device.Driver->EndpointReadStart((uint8_t)0, (uint32_t)USB_ControlRequest.wLength);
      }
    }
  }
  else
  {
    /*Assert*/
    XMC_ASSERT("USBD_HandleEP0_SETUP: read invalid setup packet size", 0);
  }
}



/**
 * Device event handler for XMC USB Driver
 *
 * The device can have several events, by which it notifies the application about the occurance of event.
 * Not all events are available on all chip series. (Power Events are only supported on XMC4500)
 *
 */
static void USBD_SignalDeviceEventHandler(XMC_USBD_EVENT_t event)
{
  uint32_t i;

  switch (event)
  {
    case XMC_USBD_EVENT_RESET:
      USB_DeviceState = (uint8_t)DEVICE_STATE_Default;
      device.ep0_state = USBD_EP0_STATE_IDLE;
      device.remote_wakeup = (uint8_t)0;
      /* Reset endpoints and configuration */
      for (i = ((uint32_t)XMC_USBD_NUM_EPS); i > 0U; i--)
      {
        device.Endpoints[((uint32_t)XMC_USBD_NUM_EPS - i)].InInUse = (uint8_t)0;
        device.Endpoints[((uint32_t)XMC_USBD_NUM_EPS - i)].OutInUse = (uint8_t)0;
        device.Endpoints[((uint32_t)XMC_USBD_NUM_EPS - i)].IsHalted = (uint8_t)0;
        if ((((uint32_t)XMC_USBD_NUM_EPS - i) != 0U) && 
           (device.Endpoints[((uint32_t)XMC_USBD_NUM_EPS - i)].IsConfigured))
        {
          if (USBD_STATUS_SUCCESS == 
          (USBD_STATUS_t)device.Driver->EndpointUnconfigure
          ((uint8_t)device.Endpoints[((uint32_t)XMC_USBD_NUM_EPS - i)].Address))
          {
            device.Endpoints[((uint32_t)XMC_USBD_NUM_EPS - i)].IsConfigured = (uint8_t)0;
            device.Endpoints[((uint32_t)XMC_USBD_NUM_EPS - i)].IsEnabled = (uint8_t)0;
          }
        }
      }
      device.configuration = (uint8_t)0;
      for (i = ((uint32_t)NUM_INTERFACES); i > 0U; i--)
      {
        device.interface_settings[((uint32_t)NUM_INTERFACES - i)] = (uint8_t)0;
      }
      device.Driver->EndpointReadStart((uint8_t)device.Endpoints[0].Address,(uint32_t)USBD_EP0_3SETUP_PKT_SIZE);
      if (NULL != device.events->reset)
      {
        device.events->reset();
      }
      break;
    case XMC_USBD_EVENT_SOF:
      if (NULL != device.events->start_of_frame)
      {
        device.events->start_of_frame();
      }
      break;
    case XMC_USBD_EVENT_CONNECT:
      if (NULL != device.events->connect)
      {
        device.events->connect();
      }
      break;
    case XMC_USBD_EVENT_DISCONNECT:
      USB_DeviceState = (uint8_t)DEVICE_STATE_Powered;
      if (NULL != device.events->disconnect)
      {
        device.events->disconnect();
      }
      break;
    case XMC_USBD_EVENT_POWER_OFF:
      USB_DeviceState = (uint8_t)DEVICE_STATE_Unattached;
      if (NULL != device.events->disconnect)
      {
        device.events->disconnect();
      }
      break;
    case XMC_USBD_EVENT_POWER_ON:
      USB_DeviceState = (uint8_t)DEVICE_STATE_Powered;
      break;
    case XMC_USBD_EVENT_REMOTE_WAKEUP:
      break;
    case XMC_USBD_EVENT_RESUME:
      USB_DeviceState = (uint8_t)device.pre_suspend_device_state;
      if (NULL != device.events->wakeup)
      {
        device.events->wakeup();
      }
      break;
    case XMC_USBD_EVENT_SUSPEND:
      device.pre_suspend_device_state = USB_DeviceState;
      USB_DeviceState = (uint8_t)DEVICE_STATE_Suspended;
      if (NULL != device.events->suspend)
      {
        device.events->suspend();
      }
      break;
    default:
      break;
  }
}

/**
 * Endpoint event handler for the XMC USB driver
 *
 * If the driver detects an event (See XMC_USBD_EP_EVENT_t) for a specified endpoint it calls this function.
 * Based on the event some further action is taken, e.g. process control request or update transfer information
 * and read data from the driver into the core buffer.
 */
static void USBD_SignalEndpointEvent_Handler(uint8_t ep_addr, XMC_USBD_EP_EVENT_t ep_event)
{
  USBD_Endpoint_t *ep;
  int32_t data_count;
  uint32_t temp_num;
  uint32_t temp_dir;

  ep =  &device.Endpoints[(ep_addr & ENDPOINT_EPNUM_MASK)];
  /* store CurrentEndpoint and direction for restore after handling */
  temp_num = (uint32_t)device.CurrentEndpoint;
  temp_dir = (uint32_t)device.CurrentDirection;
  /* select the given endpoint */
  device.CurrentEndpoint = ep_addr & ENDPOINT_EPNUM_MASK;
  device.CurrentDirection = ep_addr & (uint8_t)ENDPOINT_DIR_MASK;
  /* choose what to do based on the event */
  switch (ep_event)
  {
    case XMC_USBD_EP_EVENT_SETUP:
      ep->OutInUse = 0U;
      switch (device.CurrentEndpoint)
      {
        case 0:
          USBD_HandleEP0_SETUP();
          break;
        default:
          break;
      }
      break;
    case XMC_USBD_EP_EVENT_OUT:
      ep->IsOutRecieved = 1U;
      if (ep->OutBytesAvailable == 0U)
      {
        ep->OutOffset = 0U; /* clear offset, new data is there */
        ep->OutBytesAvailable = (uint32_t)device.Driver->EndpointRead((uint8_t)ep->Address,
                                ep->OutBuffer,ep->OutBufferLength);
      }
      ep->OutInUse = (uint8_t)0;
      switch (device.CurrentEndpoint)
      {
        case 0:
          USBD_HandleEP0_OUT();
          break;
        default:
          break;
      }
      break;
    case XMC_USBD_EP_EVENT_IN:
      /* loop write transfers */
      if (ep->InDataLeft > 0U)
      {
        data_count = device.Driver->EndpointWrite((uint8_t)ep->Address,ep->InDataBuffer,ep->InDataLeft);
        ep->InDataLeft -= (uint32_t)data_count;
        ep->InDataBuffer += (uint32_t)data_count;
        break;
      }
      else if (((uint32_t)ep->Number == 0U) && (ep->InBytesAvailable > 0U) &&
              (ep->InBytesAvailable != (uint32_t)USB_ControlRequest.wLength) &&
              ((ep->InBytesAvailable % (uint32_t)ep->MaxPacketSize) == 0U))
      {
        /* if the amount of data for endpoint 0 is exact the requested
         * amount, then no zlp has to be send */
        device.Driver->EndpointWrite((uint8_t)ep->Address,0,0U);
      }
      else
      {
      }
      ep->InBytesAvailable = 0U;
      ep->InInUse = (uint8_t)0;
      switch (device.CurrentEndpoint)
      {
        case 0:
          USBD_HandleEP0_IN();
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  device.CurrentEndpoint = (uint8_t)temp_num;
  device.CurrentDirection = (uint8_t)temp_dir;
}




