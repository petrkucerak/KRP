
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define CEC_CK_MCO1_Pin GPIO_PIN_8
#define CEC_CK_MCO1_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOI
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define LED4_Pin GPIO_PIN_15
#define LED4_GPIO_Port GPIOI
#define ARD_A2_Pin GPIO_PIN_0
#define ARD_A2_GPIO_Port GPIOA
#define ARD_A3_Pin GPIO_PIN_1
#define ARD_A3_GPIO_Port GPIOA

// NFC READER

// #define B1_Pin GPIO_PIN_13
// #define B1_GPIO_Port GPIOC

// MCU_LED1 (PA8 [CN9]) -> PJ0 [CN6]
#define NFC_LED1_GPIO_Port GPIOJ
#define NFC_LED1_Pin GPIO_PIN_0
// MCU_LED2 (PB10 [CN9]) -> PJ7 [CN6]
#define NFC_LED2_GPIO_Port GPIOJ
#define NFC_LED2_Pin GPIO_PIN_7
// MCU_LED3 (PB4 [CN9]) -> PA6 [CN6]
#define NFC_LED3_GPIO_Port GPIOA
#define NFC_LED3_Pin GPIO_PIN_6
// MCU_LED4 (PB5 [CN9]) -> PJ4 [CN6]
#define NFC_LED4_GPIO_Port GPIOJ
#define NFC_LED4_Pin GPIO_PIN_4

// IRQ_IN/OUT same pin as UART
// UART_TX/IRQ_IN (PA9 [CN5]) -> PJ5 [CN5]
#define nIRQ_IN_Pin GPIO_PIN_5
#define nIRQ_IN_GPIO_Port GPIOJ
// UART_RX/IRQ_OUT (PA10 [C9]) -> PJ3 [CN6]
#define nIRQ_OUT_Pin GPIO_PIN_3
#define nIRQ_OUT_GPIO_Port GPIOJ

// SPI_CS_NFC (PB6 [CN5]) -> PK1
#define nSPI_SS_Pin GPIO_PIN_1
#define nSPI_SS_GPIO_Port GPIOK

// SPI definitions
#define ARD_D13_Pin GPIO_PIN_0
#define ARD_D13_GPIO_Port GPIOK
#define ARD_D10_Pin GPIO_PIN_1
#define ARD_D10_GPIO_Port GPIOK
#define ARD_D12_Pin GPIO_PIN_11
#define ARD_D12_GPIO_Port GPIOJ
#define ARD_D11_Pin GPIO_PIN_10
#define ARD_D11_GPIO_Port GPIOJ

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
