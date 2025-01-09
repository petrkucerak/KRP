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
#include "gpio.h"
#include "logger.h"
#include "memorymap.h"
#include "usart.h"

#include "custom_lcd.h"
// #include "usb_device.h"
// #include "usb_host.h"
// #include "usbd_cdc_if.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void Error_Handler(void);
static void MPU_Config(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

   int32_t timeout;

   /* Wait until CPU2 boots and enters in stop mode or timeout*/
   timeout = 0xFFFF;
   while ((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0))
      ;
   if (timeout < 0) {
      Error_Handler();
   }
   /* MCU
    * Configuration--------------------------------------------------------*/

   /* Reset of all peripherals, Initializes the Flash interface and the Systick.
    */
   HAL_Init();

   /* Configure the MPU attributes as Write Through for SDRAM*/
   MPU_Config();

   /* Configure the system clock */
   SystemClock_Config();
   /* When system initialization is finished, Cortex-M7 will release Cortex-M4
by means of HSEM notification */
   /*HW semaphore Clock enable*/
   __HAL_RCC_HSEM_CLK_ENABLE();
   /*Take HSEM */
   HAL_HSEM_FastTake(HSEM_ID_0);
   /*Release HSEM in order to notify the CPU2(CM4)*/
   HAL_HSEM_Release(HSEM_ID_0, 0);
   /* wait until CPU2 wakes up from stop mode */
   timeout = 0xFFFF;
   while ((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0))
      ;
   if (timeout < 0) {
      Error_Handler();
   }

   if (BSP_SDRAM_Init(0) != BSP_ERROR_NONE) {
      Error_Handler();
   }

   /* Initialize all configured peripherals */
   MX_GPIO_Init();
   MX_USART1_UART_Init();

   // MX_USB_DEVICE_Init();
   // MX_USB_HOST_Init();

   BSP_LED_Init(LED1);
   BSP_LED_Init(LED2);

   /* Initialize the SDRAM */
   BSP_SDRAM_Init(0);

   /* Init Touch Screen */
   if (TS_Init() != BSP_ERROR_NONE) {
      Error_Handler();
   }

   /* Initialize the LCD   */
   if (LCD_Init() != BSP_ERROR_NONE) {
      Error_Handler();
   }

   LCD_InitScreen();

   uint8_t tx_buff[] = "Hello World! This is V-COM Port\r\n";
   uint8_t tx_buff_len = sizeof(tx_buff);

   /* Infinite loop */
   while (1) {

      // BSP_LED_On(LED1);
      // BSP_LED_Off(LED2);
      HAL_Delay(1000);

      // BSP_LED_Off(LED1);
      // BSP_LED_On(LED2);
      HAL_Delay(1000);

      // CDC_Transmit_HS(tx_buff, tx_buff_len);
      HAL_UART_Transmit(&huart1, tx_buff, tx_buff_len, 1000);
      // platformLog("Hello world!\r\n");
   }
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 400000000 (CM7 CPU Clock)
 *            HCLK(Hz)                       = 200000000 (CM4 CPU, AXI and
 * AHBs Clock) AHB Prescaler                  = 2 D1 APB3 Prescaler = 2 (APB3
 * Clock  100MHz) D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz) D2
 * APB2 Prescaler              = 2 (APB2 Clock  100MHz) D3 APB4 Prescaler = 2
 * (APB4 Clock  100MHz) HSE Frequency(Hz)              = 25000000 PLL_M = 5
 *            PLL_N                          = 160
 *            PLL_P                          = 2
 *            PLL_Q                          = 4
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Flash Latency(WS)              = 4
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
   RCC_ClkInitTypeDef RCC_ClkInitStruct;
   RCC_OscInitTypeDef RCC_OscInitStruct;
   HAL_StatusTypeDef ret = HAL_OK;

   /*!< Supply configuration update enable */
   HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

   /* The voltage scaling allows optimizing the power consumption when the
    device is clocked below the maximum system frequency, to update the
    voltage scaling value regarding system frequency refer to product
    datasheet.  */
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

   while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
   }

   /* Enable HSE Oscillator and activate PLL with HSE as source */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
   RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
   RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

   RCC_OscInitStruct.PLL.PLLM = 5;
   RCC_OscInitStruct.PLL.PLLN = 160;
   RCC_OscInitStruct.PLL.PLLFRACN = 0;
   RCC_OscInitStruct.PLL.PLLP = 2;
   RCC_OscInitStruct.PLL.PLLR = 2;
   RCC_OscInitStruct.PLL.PLLQ = 4;

   RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
   RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
   ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
   if (ret != HAL_OK) {
      Error_Handler();
   }

   /* Select PLL as system clock source and configure  bus clocks dividers */
   RCC_ClkInitStruct.ClockType =
       (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 |
        RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1);

   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
   RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
   RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
   ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
   if (ret != HAL_OK) {
      Error_Handler();
   }

   /*
    Note : The activation of the I/O Compensation Cell is recommended with
    communication  interfaces (GPIO, SPI, FMC, QSPI ...)  when  operating at
    high frequencies(please refer to product datasheet) The I/O Compensation
    Cell activation  procedure requires :
    - The activation of the CSI clock
    - The activation of the SYSCFG clock
    - Enabling the I/O Compensation Cell : setting bit[0] of register
    SYSCFG_CCCSR
    */

   /*activate CSI clock mondatory for I/O Compensation Cell*/
   __HAL_RCC_CSI_ENABLE();

   /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
   __HAL_RCC_SYSCFG_CLK_ENABLE();

   /* Enables the I/O Compensation Cell */
   HAL_EnableCompensationCell();
}

/**
 * @brief  Configure the MPU attributes as Write Through for External SDRAM.
 * @note   The Base Address is 0xD0000000 .
 *         The Configured Region Size is 32MB because same as SDRAM size.
 * @param  None
 * @retval None
 */
static void MPU_Config(void)
{
   MPU_Region_InitTypeDef MPU_InitStruct;

   /* Disable the MPU */
   HAL_MPU_Disable();

   /* Configure the MPU as Strongly ordered for not defined regions */
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.BaseAddress = 0x00;
   MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
   MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER0;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.SubRegionDisable = 0x87;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   /* Configure the MPU attributes as WT for SDRAM */
   MPU_InitStruct.Enable = MPU_REGION_ENABLE;
   MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
   MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
   MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
   MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
   MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
   MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
   MPU_InitStruct.Number = MPU_REGION_NUMBER1;
   MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
   MPU_InitStruct.SubRegionDisable = 0x00;
   MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

   HAL_MPU_ConfigRegion(&MPU_InitStruct);

   /* Enable the MPU */
   HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief Error Handler
 * @retval None
 */
void Error_Handler(void)
{
   /* User can add his own implementation to report the HAL error return state
    */
   BSP_LED_On(LED2);
   while (1) {
      ;
   } /* Blocking on error */
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
   while (1) {
   }
}
#endif /* USE_FULL_ASSERT */
