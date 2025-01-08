
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/

/** Configure pins
 */
void MX_GPIO_Init(void)
{

   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOI_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
   // NFC
   __HAL_RCC_GPIOJ_CLK_ENABLE();

   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(GPIOI, LED3_Pin | LED4_Pin, GPIO_PIN_RESET);

   /*Configure GPIO pins : LED3_Pin LED4_Pin */
   GPIO_InitStruct.Pin = LED3_Pin | LED4_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

   // ~~~ NFC GPIO configuration ~~~
   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(GPIOJ,
                     NFC_LED1_Pin | NFC_LED2_Pin | NFC_LED4_Pin | nIRQ_IN_Pin,
                     GPIO_PIN_RESET);
   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(GPIOA, NFC_LED3_Pin, GPIO_PIN_RESET);

   // LEDs
   /*Configure GPIO pins : NFC_LED1_Pin NFC_LED2_Pin NFC_LED4_Pin */
   GPIO_InitStruct.Pin = NFC_LED1_Pin | NFC_LED2_Pin | NFC_LED4_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

   /*Configure GPIO pins : NFC_LED3_Pin */
   GPIO_InitStruct.Pin = NFC_LED3_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   // nIRQ
   /*Configure GPIO pin : nIRQ_OUT_Pin */
   GPIO_InitStruct.Pin = nIRQ_OUT_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(nIRQ_OUT_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pins : nIRQ_IN_Pin */
   GPIO_InitStruct.Pin = nIRQ_IN_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(nIRQ_IN_GPIO_Port, &GPIO_InitStruct);
}
