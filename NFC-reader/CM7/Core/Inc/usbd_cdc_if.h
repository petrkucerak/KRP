
/**
 ******************************************************************************
 * @file           : usbd_cdc_if.h
 * @version        : v1.0_Cube
 * @brief          : Header for usbd_cdc_if.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H__
#define __USBD_CDC_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
 * @brief For Usb device.
 * @{
 */

/** @defgroup USBD_CDC_IF USBD_CDC_IF
 * @brief Usb VCP device module
 * @{
 */

/** @defgroup USBD_CDC_IF_Exported_Defines USBD_CDC_IF_Exported_Defines
 * @brief Defines.
 * @{
 */
/* Define size for the receive and transmit buffer over CDC */
#define APP_RX_DATA_SIZE 2048
#define APP_TX_DATA_SIZE 2048

/**
 * @}
 */

/** @defgroup USBD_CDC_IF_Exported_Types USBD_CDC_IF_Exported_Types
 * @brief Types.
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_CDC_IF_Exported_Macros USBD_CDC_IF_Exported_Macros
 * @brief Aliases.
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
 * @brief Public variables.
 * @{
 */

/** CDC Interface callback. */
extern USBD_CDC_ItfTypeDef USBD_Interface_fops_HS;

/**
 * @}
 */

/** @defgroup USBD_CDC_IF_Exported_FunctionsPrototype
 * USBD_CDC_IF_Exported_FunctionsPrototype
 * @brief Public functions declaration.
 * @{
 */

uint8_t CDC_Transmit_HS(uint8_t *Buf, uint16_t Len);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_IF_H__ */
