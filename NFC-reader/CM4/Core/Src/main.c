
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
#include "string.h"

/* Private includes ----------------------------------------------------------*/
#include "demo.h"
#include "platform.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi5;
UART_HandleTypeDef huart1;

/* Private function prototypes -----------------------------------------------*/
static void MX_USART1_UART_Init(void);
static void MX_SPI5_Init(void);

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
   MX_USART1_UART_Init();
   MX_SPI5_Init();

   // /* Initialize log module */
   logUsartInit(&huart1);

   platformLog("\r\nWelcome NFC READER APP\r\n");

   /* Initalize RFAL */
   if (!demoIni()) {
      /*
       * in case the rfal initalization failed signal it by flashing all LED
       * and stoping all operations
       */
      platformLog("Initialization failed..\r\n");
      while (1) {
         platformLedToogle(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
         platformLedToogle(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
         platformLedToogle(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
         platformLedToogle(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
         platformDelay(100);
      }
   } else {
      platformLog("Initialization succeeded..\r\n");
      for (int i = 0; i < 6; i++) {
         platformLedToogle(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
         platformLedToogle(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
         platformLedToogle(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
         platformLedToogle(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
         platformDelay(200);
      }

      platformLedOff(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
      platformLedOff(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
      platformLedOff(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
      platformLedOff(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
   }

   /* Infinite loop */

   while (1) {
      HAL_GPIO_WritePin(NFC_LED1_GPIO_Port, NFC_LED1_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(NFC_LED2_GPIO_Port, NFC_LED2_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(NFC_LED3_GPIO_Port, NFC_LED3_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(NFC_LED4_GPIO_Port, NFC_LED4_Pin, GPIO_PIN_SET);

      HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
      HAL_Delay(1000);

      HAL_GPIO_WritePin(NFC_LED1_GPIO_Port, NFC_LED1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(NFC_LED2_GPIO_Port, NFC_LED2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(NFC_LED3_GPIO_Port, NFC_LED3_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(NFC_LED4_GPIO_Port, NFC_LED4_Pin, GPIO_PIN_RESET);

      HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
      HAL_Delay(1000);

      // demoCycle();
   }
}

/**
 * @brief SPI5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI5_Init(void)
{

   /* SPI5 parameter configuration*/
   hspi5.Instance = SPI5;
   hspi5.Init.Mode = SPI_MODE_MASTER;
   hspi5.Init.Direction = SPI_DIRECTION_2LINES;
   hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
   hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
   hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
   hspi5.Init.NSS = SPI_NSS_SOFT;
   hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
   hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
   hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
   hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
   hspi5.Init.CRCPolynomial = 7;
   hspi5.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
   hspi5.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
   if (HAL_SPI_Init(&hspi5) != HAL_OK) {
      Error_Handler();
   }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{

   GPIO_InitTypeDef GPIO_InitStruct = {0};
   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
   if (spiHandle->Instance == SPI5) {

      /** Initializes the peripherals clock
       */
      PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI5;
      PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
         Error_Handler();
      }

      /* SPI5 clock enable */
      __HAL_RCC_SPI5_CLK_ENABLE();

      __HAL_RCC_GPIOK_CLK_ENABLE();
      __HAL_RCC_GPIOJ_CLK_ENABLE();
      /**SPI5 GPIO Configuration
      PK0     ------> SPI5_SCK
      PK1     ------> SPI5_NSS
      PJ11     ------> SPI5_MISO
      PJ10     ------> SPI5_MOSI
      */
      GPIO_InitStruct.Pin = ARD_D13_Pin | ARD_D10_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
      HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = ARD_D12_Pin | ARD_D11_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
      HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
   }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{

   if (spiHandle->Instance == SPI5) {
      /* USER CODE BEGIN SPI5_MspDeInit 0 */

      /* USER CODE END SPI5_MspDeInit 0 */
      /* Peripheral clock disable */
      __HAL_RCC_SPI5_CLK_DISABLE();

      /**SPI5 GPIO Configuration
      PK0     ------> SPI5_SCK
      PK1     ------> SPI5_NSS
      PJ11     ------> SPI5_MISO
      PJ10     ------> SPI5_MOSI
      */
      HAL_GPIO_DeInit(GPIOK, ARD_D13_Pin | ARD_D10_Pin);

      HAL_GPIO_DeInit(GPIOJ, ARD_D12_Pin | ARD_D11_Pin);

      /* USER CODE BEGIN SPI5_MspDeInit 1 */

      /* USER CODE END SPI5_MspDeInit 1 */
   }
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

   huart1.Instance = USART1;
   huart1.Init.BaudRate = 115200;
   huart1.Init.WordLength = UART_WORDLENGTH_8B;
   huart1.Init.StopBits = UART_STOPBITS_1;
   huart1.Init.Parity = UART_PARITY_NONE;
   huart1.Init.Mode = UART_MODE_TX_RX;
   huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   huart1.Init.OverSampling = UART_OVERSAMPLING_16;
   huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
   huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
   huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
   if (HAL_UART_Init(&huart1) != HAL_OK) {
      Error_Handler();
   }
   if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) !=
       HAL_OK) {
      Error_Handler();
   }
   if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) !=
       HAL_OK) {
      Error_Handler();
   }
   if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) {
      Error_Handler();
   }
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

   GPIO_InitTypeDef GPIO_InitStruct = {0};
   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
   if (uartHandle->Instance == USART1) {
      /* USER CODE BEGIN USART1_MspInit 0 */

      /* USER CODE END USART1_MspInit 0 */

      /** Initializes the peripherals clock
       */
      PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
      PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
         Error_Handler();
      }

      /* USART1 clock enable */
      __HAL_RCC_USART1_CLK_ENABLE();

      __HAL_RCC_GPIOA_CLK_ENABLE();
      /**USART1 GPIO Configuration
      PA10     ------> USART1_RX
      PA9     ------> USART1_TX
      */
      GPIO_InitStruct.Pin = STLINK_TX_Pin | STLINK_RX_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
   }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

   if (uartHandle->Instance == USART1) {
      /* Peripheral clock disable */
      __HAL_RCC_USART1_CLK_DISABLE();
   }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{

   /* User can add his own implementation to report the HAL error return
    * state
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
      number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
      file, line) */
}
#endif /* USE_FULL_ASSERT */
