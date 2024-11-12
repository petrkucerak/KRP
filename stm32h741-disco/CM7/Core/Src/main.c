/* USER CODE BEGIN Header */
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <stm32h747i_discovery_ts.h>
#include <stm32h7xx_hal_dsi.h>
#include <stm32h7xx_hal_ltdc.h>
#include <string.h>
#include <otm8009a.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

extern LTDC_HandleTypeDef hlcd_ltdc;
extern DSI_HandleTypeDef hlcd_dsi;
DSI_VidCfgTypeDef hdsivideo_handle;
DSI_CmdCfgTypeDef CmdCfg;
DSI_LPCmdTypeDef LPCmd;
DSI_PLLInitTypeDef dsiPllInit;
static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
OTM8009A_Object_t *pObj;

typedef enum {
   FRONT_SCREEN,
   TURNON_SCENE,
   TIMER_CONFIG_SCENE,
   WAITING_SCENE
} Scene_t;
typedef enum { PUSH_BUTTON, TIMER_BUTTON, NONE } Button_type_t;

typedef struct {
   Scene_t scene;
   uint8_t _delay;
   char status_message[50];
   char title[50];
   uint32_t status_color;
   uint16_t progress_bar;
   uint32_t timer;
   uint32_t config_timer;
   uint32_t timer_left;
   uint32_t timer_start_time;
   uint32_t button_left_color;
   Button_type_t button_left_type;
   uint32_t button_right_color;
   Button_type_t button_right_type;
} App_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define VSYNC 1
#define VBP 1
#define VFP 1
#define VACT 480
#define HSYNC 1
#define HBP 1
#define HFP 1
#define HACT 800

#define TS_ACCURACY 2
#define TS_INSTANCE 0

#define SECOND 1000

#define APP_COLOR_BACKGROUND UTIL_LCD_COLOR_CUSTOM_Stone
#define APP_COLOR_RED UTIL_LCD_COLOR_RED
#define APP_COLOR_BLUE UTIL_LCD_COLOR_CUSTOM_Blue
#define APP_COLOR_TEXT UTIL_LCD_COLOR_WHITE
#define APP_COLOR_GREEN UTIL_LCD_COLOR_DARKGREEN
#define APP_COLOR_YELLOW UTIL_LCD_COLOR_CUSTOM_Yellow
#define APP_COLOR_STONE UTIL_LCD_COLOR_BLACK

#define LAYER0_ADDRESS (LCD_FB_START_ADDRESS)

#define DELAY 1000

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static int32_t pending_buffer = -1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
   /* USER CODE BEGIN 1 */

   /* USER CODE END 1 */
   /* USER CODE BEGIN Boot_Mode_Sequence_0 */
   int32_t timeout;
   /* USER CODE END Boot_Mode_Sequence_0 */

   /* USER CODE BEGIN Boot_Mode_Sequence_1 */
   /* Wait until CPU2 boots and enters in stop mode or timeout*/
   timeout = 0xFFFF;
   while ((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0))
      ;
   if (timeout < 0) {
      Error_Handler();
   }
   /* USER CODE END Boot_Mode_Sequence_1 */
   /* MCU
    * Configuration--------------------------------------------------------*/

   /* Reset of all peripherals, Initializes the Flash interface and the Systick.
    */
   HAL_Init();

   /* USER CODE BEGIN Init */

   /* USER CODE END Init */

   /* Configure the system clock */
   SystemClock_Config();
   /* USER CODE BEGIN Boot_Mode_Sequence_2 */
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
   /* USER CODE END Boot_Mode_Sequence_2 */

   /* USER CODE BEGIN SysInit */

   /* USER CODE END SysInit */

   /* Initialize all configured peripherals */
   MX_GPIO_Init();
   /* USER CODE BEGIN 2 */

   /* USER CODE END 2 */

   /* Infinite loop */
   /* USER CODE BEGIN WHILE */
   while (1) {
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
      HAL_Delay(DELAY);
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
      HAL_Delay(DELAY);
      /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */
   }
   /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

   /** Supply configuration update enable
    */
   HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

   /** Configure the main internal regulator output voltage
    */
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

   while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
   }

   /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
   RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
   RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
      Error_Handler();
   }

   /** Initializes the CPU, AHB and APB buses clocks
    */
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                 RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
   RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
   RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
   RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
      Error_Handler();
   }
   HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
   /* USER CODE BEGIN Error_Handler_Debug */
   /* User can add his own implementation to report the HAL error return state
    */
   __disable_irq();
   while (1) {
   }
   /* USER CODE END Error_Handler_Debug */
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
   /* USER CODE BEGIN 6 */
   /* User can add his own implementation to report the file name and line
      number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
      line) */
   /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
