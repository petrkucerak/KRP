
/**
 ******************************************************************************
 * @file            : usb_host.c
 * @version         : v1.0_Cube
 * @brief           : This file implements the USB Host
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_cdc.h"
#include "usbh_core.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostHS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */

/**
 * Init USB host library, add supported class and start the library
 * @retval None
 */
void MX_USB_HOST_Init(void)
{

   /* Init host Library, add supported class and start the library. */
   if (USBH_Init(&hUsbHostHS, USBH_UserProcess, HOST_HS) != USBH_OK) {
      Error_Handler();
   }
   if (USBH_RegisterClass(&hUsbHostHS, USBH_CDC_CLASS) != USBH_OK) {
      Error_Handler();
   }
   if (USBH_Start(&hUsbHostHS) != USBH_OK) {
      Error_Handler();
   }
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
   /* USB Host Background task */
   USBH_Process(&hUsbHostHS);
}
/*
 * user callback definition
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{

   switch (id) {
   case HOST_USER_SELECT_CONFIGURATION:
      break;

   case HOST_USER_DISCONNECTION:
      Appli_state = APPLICATION_DISCONNECT;
      break;

   case HOST_USER_CLASS_ACTIVE:
      Appli_state = APPLICATION_READY;
      break;

   case HOST_USER_CONNECTION:
      Appli_state = APPLICATION_START;
      break;

   default:
      break;
   }
}

/**
 * @}
 */

/**
 * @}
 */
