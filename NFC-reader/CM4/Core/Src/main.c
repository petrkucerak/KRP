
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
// #include "demo.h"
// #include "platform.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// SPI_HandleTypeDef hspi1;

// UART_HandleTypeDef huart2;
// /* Private function prototypes
// -----------------------------------------------*/ static void
// MX_USART2_UART_Init(void); static void MX_SPI1_Init(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

   /*HW semaphore Clock enable*/
   __HAL_RCC_HSEM_CLK_ENABLE();
   /* Activate HSEM notification for Cortex-M4*/
   HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
   /*
   Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7
   to perform system initialization (system clock config, external memory
   configuration.. )
   */
   HAL_PWREx_ClearPendingEvent();
   HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE,
                           PWR_D2_DOMAIN);
   /* Clear HSEM flag */
   __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

   /* MCU
    * Configuration--------------------------------------------------------*/

   /* Reset of all peripherals, Initializes the Flash interface and the Systick.
    */
   HAL_Init();

   /* Initialize all configured peripherals */
   MX_GPIO_Init();
   MX_ADC1_Init();
   // MX_USART2_UART_Init();
   // MX_SPI1_Init();

   // /* Initialize log module */
   // logUsartInit(&huart2);

   // platformLog("\r\nWelcome to X-NUCLEO-NFC03A1 (SPI Interface)\r\n");

   // /* Initalize RFAL */
   // if (!demoIni()) {
   //    /*
   //     * in case the rfal initalization failed signal it by flashing all LED
   //     * and stoping all operations
   //     */
   //    platformLog("Initialization failed..\r\n");
   //    while (1) {
   //       platformLedToogle(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
   //       platformLedToogle(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
   //       platformLedToogle(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
   //       platformLedToogle(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
   //       platformDelay(100);
   //    }
   // } else {
   //    platformLog("Initialization succeeded..\r\n");
   //    for (int i = 0; i < 6; i++) {
   //       platformLedToogle(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
   //       platformLedToogle(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
   //       platformLedToogle(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
   //       platformLedToogle(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
   //       platformDelay(200);
   //    }

   //    platformLedOff(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
   //    platformLedOff(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
   //    platformLedOff(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
   //    platformLedOff(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
   // }

   /* Infinite loop */

   while (1) {
      HAL_GPIO_WritePin(NFC_LED1_GPIO_Port, NFC_LED1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(NFC_LED2_GPIO_Port, NFC_LED2_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(NFC_LED3_GPIO_Port, NFC_LED3_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(NFC_LED4_GPIO_Port, NFC_LED4_Pin, GPIO_PIN_SET);
      HAL_Delay(1000);
      HAL_GPIO_WritePin(NFC_LED1_GPIO_Port, NFC_LED1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(NFC_LED2_GPIO_Port, NFC_LED2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(NFC_LED3_GPIO_Port, NFC_LED3_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(NFC_LED4_GPIO_Port, NFC_LED4_Pin, GPIO_PIN_RESET);
      HAL_Delay(1000);

      // demoCycle();
   }
}

// /**
//  * @brief SPI1 Initialization Function
//  * @param None
//  * @retval None
//  */
// static void MX_SPI1_Init(void)
// {

//    /* SPI1 parameter configuration*/
//    hspi1.Instance = SPI1;
//    hspi1.Init.Mode = SPI_MODE_MASTER;
//    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
//    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
//    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
//    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
//    hspi1.Init.NSS = SPI_NSS_SOFT;
//    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
//    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
//    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//    hspi1.Init.CRCPolynomial = 7;
//    hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
//    hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
//    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
//       Error_Handler();
//    }
// }

// /**
//  * @brief USART2 Initialization Function
//  * @param None
//  * @retval None
//  */
// static void MX_USART2_UART_Init(void)
// {

//    huart2.Instance = USART2;
//    huart2.Init.BaudRate = 115200;
//    huart2.Init.WordLength = UART_WORDLENGTH_8B;
//    huart2.Init.StopBits = UART_STOPBITS_1;
//    huart2.Init.Parity = UART_PARITY_NONE;
//    huart2.Init.Mode = UART_MODE_TX_RX;
//    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//    if (HAL_UART_Init(&huart2) != HAL_OK) {
//       Error_Handler();
//    }
// }

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{

   /* User can add his own implementation to report the HAL error return state
    */
   __disable_irq();
   while (1) {
   }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{

   /* User can add his own implementation to report the file name and line
      number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
      line) */
}
#endif /* USE_FULL_ASSERT */
