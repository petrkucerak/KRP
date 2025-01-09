/**
 * @file custom_lcd.c
 * @brief Implementation of LCD initialization and configuration functions.
 * @author Petr Kucera (kucerp28@fel.cvut.cz)
 */
#include "custom_lcd.h"

extern LTDC_HandleTypeDef hlcd_ltdc;
extern DSI_HandleTypeDef hlcd_dsi;
DSI_VidCfgTypeDef hdsivideo_handle;
DSI_CmdCfgTypeDef CmdCfg;
DSI_LPCmdTypeDef LPCmd;
DSI_PLLInitTypeDef dsiPllInit;
static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
OTM8009A_Object_t *pObj;

const LCD_UTILS_Drv_t LCD_UTIL_Driver = {
    BSP_LCD_DrawBitmap,     BSP_LCD_FillRGBRect,   BSP_LCD_DrawHLine,
    BSP_LCD_DrawVLine,      BSP_LCD_FillRect,      BSP_LCD_ReadPixel,
    BSP_LCD_WritePixel,     LCD_GetXSize,          LCD_GetYSize,
    BSP_LCD_SetActiveLayer, BSP_LCD_GetPixelFormat};

static int32_t pending_buffer = -1;

/**
 * @brief  Gets the LCD X size.
 * @param  Instance  LCD Instance
 * @param  XSize     LCD width
 * @retval BSP status
 */
int32_t LCD_GetXSize(uint32_t Instance, uint32_t *XSize)
{
   *XSize = Lcd_Ctx[0].XSize;

   return BSP_ERROR_NONE;
}

/**
 * @brief  Gets the LCD Y size.
 * @param  Instance  LCD Instance
 * @param  YSize     LCD Height
 * @retval BSP status
 */
int32_t LCD_GetYSize(uint32_t Instance, uint32_t *YSize)
{
   *YSize = Lcd_Ctx[0].YSize;

   return BSP_ERROR_NONE;
}
/**
 * @brief  End of Refresh DSI callback.
 * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
 *               the configuration information for the DSI.
 * @retval None
 */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
   if (pending_buffer >= 0) {
      pending_buffer = -1;
   }
}

/**
 * @brief  Initializes the DSI LCD.
 * The ititialization is done as below:
 *     - DSI PLL ititialization
 *     - DSI ititialization
 *     - LTDC ititialization
 *     - OTM8009A LCD Display IC Driver ititialization
 * @param  None
 * @retval LCD state
 */
uint8_t LCD_Init(void)
{
   DSI_PHY_TimerTypeDef PhyTimings;
   OTM8009A_IO_t IOCtx;
   static OTM8009A_Object_t OTM8009AObj;
   static void *Lcd_CompObj = NULL;

   /* Toggle Hardware Reset of the DSI LCD using
    its XRES signal (active low) */
   BSP_LCD_Reset(0);

   /* Call first MSP Initialize only in case of first initialization
    * This will set IP blocks LTDC, DSI and DMA2D
    * - out of reset
    * - clocked
    * - NVIC IRQ related to IP blocks enabled
    */
   LCD_MspInit();

   /* LCD clock configuration */
   /* LCD clock configuration */
   /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
   /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
   /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/19 = 42 Mhz */
   /* LTDC clock frequency = PLLLCDCLK = 42 Mhz */
   PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
   PeriphClkInitStruct.PLL3.PLL3M = 5;
   PeriphClkInitStruct.PLL3.PLL3N = 100;
   PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
   PeriphClkInitStruct.PLL3.PLL3P = 2;
   PeriphClkInitStruct.PLL3.PLL3Q = 2;
   PeriphClkInitStruct.PLL3.PLL3R = 25;
   PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
   PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
   HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

   /* Base address of DSI Host/Wrapper registers to be set before calling
    * De-Init */
   hlcd_dsi.Instance = DSI;

   HAL_DSI_DeInit(&(hlcd_dsi));

   dsiPllInit.PLLNDIV = 100;
   dsiPllInit.PLLIDF = DSI_PLL_IN_DIV5;
   dsiPllInit.PLLODF = DSI_PLL_OUT_DIV1;

   hlcd_dsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
   hlcd_dsi.Init.TXEscapeCkdiv = 0x4;

   HAL_DSI_Init(&(hlcd_dsi), &(dsiPllInit));

   /* Configure the DSI for Command mode */
   CmdCfg.VirtualChannelID = 0;
   CmdCfg.HSPolarity = DSI_HSYNC_ACTIVE_HIGH;
   CmdCfg.VSPolarity = DSI_VSYNC_ACTIVE_HIGH;
   CmdCfg.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
   CmdCfg.ColorCoding = DSI_RGB888;
   CmdCfg.CommandSize = HACT;
   CmdCfg.TearingEffectSource = DSI_TE_DSILINK;
   CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
   CmdCfg.VSyncPol = DSI_VSYNC_FALLING;
   CmdCfg.AutomaticRefresh = DSI_AR_DISABLE;
   CmdCfg.TEAcknowledgeRequest = DSI_TE_ACKNOWLEDGE_ENABLE;
   HAL_DSI_ConfigAdaptedCommandMode(&hlcd_dsi, &CmdCfg);

   LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_ENABLE;
   LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_ENABLE;
   LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_ENABLE;
   LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_ENABLE;
   LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_ENABLE;
   LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_ENABLE;
   LPCmd.LPGenLongWrite = DSI_LP_GLW_ENABLE;
   LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_ENABLE;
   LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_ENABLE;
   LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_ENABLE;
   LPCmd.LPDcsLongWrite = DSI_LP_DLW_ENABLE;
   HAL_DSI_ConfigCommand(&hlcd_dsi, &LPCmd);

   /* Initialize LTDC */
   LTDC_Init();

   /* Start DSI */
   HAL_DSI_Start(&(hlcd_dsi));

   /* Configure DSI PHY HS2LP and LP2HS timings */
   PhyTimings.ClockLaneHS2LPTime = 50;
   PhyTimings.ClockLaneLP2HSTime = 50;
   PhyTimings.DataLaneHS2LPTime = 50;
   PhyTimings.DataLaneLP2HSTime = 50;
   PhyTimings.DataLaneMaxReadTime = 0;
   PhyTimings.StopWaitTime = 20;
   HAL_DSI_ConfigPhyTimer(&hlcd_dsi, &PhyTimings);

   /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver) */
   IOCtx.Address = 0;
   IOCtx.GetTick = BSP_GetTick;
   IOCtx.WriteReg = DSI_IO_Write;
   IOCtx.ReadReg = DSI_IO_Read;
   OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx);
   Lcd_CompObj = (&OTM8009AObj);
   OTM8009A_Init(Lcd_CompObj, OTM8009A_COLMOD_RGB888,
                 LCD_ORIENTATION_LANDSCAPE);

   LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_DISABLE;
   LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_DISABLE;
   LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_DISABLE;
   LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_DISABLE;
   LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_DISABLE;
   LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_DISABLE;
   LPCmd.LPGenLongWrite = DSI_LP_GLW_DISABLE;
   LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_DISABLE;
   LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_DISABLE;
   LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_DISABLE;
   LPCmd.LPDcsLongWrite = DSI_LP_DLW_DISABLE;
   HAL_DSI_ConfigCommand(&hlcd_dsi, &LPCmd);

   HAL_DSI_ConfigFlowControl(&hlcd_dsi, DSI_FLOW_CONTROL_BTA);
   HAL_DSI_ForceRXLowPower(&hlcd_dsi, ENABLE);

   return BSP_ERROR_NONE;
}

/**
 * @brief
 * @param  None
 * @retval None
 */
void LTDC_Init(void)
{
   /* DeInit */
   hlcd_ltdc.Instance = LTDC;
   HAL_LTDC_DeInit(&hlcd_ltdc);

   /* LTDC Config */
   /* Timing and polarity */
   hlcd_ltdc.Init.HorizontalSync = HSYNC;
   hlcd_ltdc.Init.VerticalSync = VSYNC;
   hlcd_ltdc.Init.AccumulatedHBP = HSYNC + HBP;
   hlcd_ltdc.Init.AccumulatedVBP = VSYNC + VBP;
   hlcd_ltdc.Init.AccumulatedActiveH = VSYNC + VBP + VACT;
   hlcd_ltdc.Init.AccumulatedActiveW = HSYNC + HBP + HACT;
   hlcd_ltdc.Init.TotalHeigh = VSYNC + VBP + VACT + VFP;
   hlcd_ltdc.Init.TotalWidth = HSYNC + HBP + HACT + HFP;

   /* background value */
   hlcd_ltdc.Init.Backcolor.Blue = 0;
   hlcd_ltdc.Init.Backcolor.Green = 0;
   hlcd_ltdc.Init.Backcolor.Red = 0;

   /* Polarity */
   hlcd_ltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
   hlcd_ltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
   hlcd_ltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
   hlcd_ltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
   hlcd_ltdc.Instance = LTDC;

   HAL_LTDC_Init(&hlcd_ltdc);
}

/**
 * @brief  Initializes the LCD layers.
 * @param  LayerIndex: Layer foreground or background
 * @param  FB_Address: Layer frame buffer
 * @retval None
 */
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
   LTDC_LayerCfgTypeDef layercfg;

   /* Layer Init */
   layercfg.WindowX0 = 0;
   layercfg.WindowX1 = Lcd_Ctx[0].XSize;
   layercfg.WindowY0 = 0;
   layercfg.WindowY1 = Lcd_Ctx[0].YSize;
   layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
   layercfg.FBStartAdress = Address;
   layercfg.Alpha = 255;
   layercfg.Alpha0 = 0;
   layercfg.Backcolor.Blue = 0;
   layercfg.Backcolor.Green = 0;
   layercfg.Backcolor.Red = 0;
   layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
   layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
   layercfg.ImageWidth = Lcd_Ctx[0].XSize;
   layercfg.ImageHeight = Lcd_Ctx[0].YSize;

   HAL_LTDC_ConfigLayer(&hlcd_ltdc, &layercfg, LayerIndex);
}

/**
 * @brief  DCS or Generic short/long write command
 * @param  ChannelNbr Virtual channel ID
 * @param  Reg Register to be written
 * @param  pData pointer to a buffer of data to be write
 * @param  Size To precise command to be used (short or long)
 * @retval BSP status
 */
int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData,
                     uint16_t Size)
{
   int32_t ret = BSP_ERROR_NONE;

   if (Size <= 1U) {
      if (HAL_DSI_ShortWrite(&hlcd_dsi, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1,
                             Reg, (uint32_t)pData[Size]) != HAL_OK) {
         ret = BSP_ERROR_BUS_FAILURE;
      }
   } else {
      if (HAL_DSI_LongWrite(&hlcd_dsi, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size,
                            (uint32_t)Reg, pData) != HAL_OK) {
         ret = BSP_ERROR_BUS_FAILURE;
      }
   }

   return ret;
}

/**
 * @brief  DCS or Generic read command
 * @param  ChannelNbr Virtual channel ID
 * @param  Reg Register to be read
 * @param  pData pointer to a buffer to store the payload of a read back
 * operation.
 * @param  Size  Data size to be read (in byte).
 * @retval BSP status
 */
int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData,
                    uint16_t Size)
{
   int32_t ret = BSP_ERROR_NONE;

   if (HAL_DSI_Read(&hlcd_dsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ,
                    Reg, pData) != HAL_OK) {
      ret = BSP_ERROR_BUS_FAILURE;
   }

   return ret;
}

/**
 * @brief  Initializes the LCD peripherals (MSP initialization).
 * @retval None
 */
void LCD_MspInit(void)
{
   /** @brief Enable the LTDC clock */
   __HAL_RCC_LTDC_CLK_ENABLE();

   /** @brief Toggle Sw reset of LTDC IP */
   __HAL_RCC_LTDC_FORCE_RESET();
   __HAL_RCC_LTDC_RELEASE_RESET();

   /** @brief Enable the DMA2D clock */
   __HAL_RCC_DMA2D_CLK_ENABLE();

   /** @brief Toggle Sw reset of DMA2D IP */
   __HAL_RCC_DMA2D_FORCE_RESET();
   __HAL_RCC_DMA2D_RELEASE_RESET();

   /** @brief Enable DSI Host and wrapper clocks */
   __HAL_RCC_DSI_CLK_ENABLE();

   /** @brief Soft Reset the DSI Host and wrapper */
   __HAL_RCC_DSI_FORCE_RESET();
   __HAL_RCC_DSI_RELEASE_RESET();

   /** @brief NVIC configuration for LTDC interrupt that is now enabled */
   HAL_NVIC_SetPriority(LTDC_IRQn, 9, 0xf);
   HAL_NVIC_EnableIRQ(LTDC_IRQn);

   /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
   HAL_NVIC_SetPriority(DMA2D_IRQn, 9, 0xf);
   HAL_NVIC_EnableIRQ(DMA2D_IRQn);

   /** @brief NVIC configuration for DSI interrupt that is now enabled */
   HAL_NVIC_SetPriority(DSI_IRQn, 9, 0xf);
   HAL_NVIC_EnableIRQ(DSI_IRQn);
}

/**
 * @brief Init touch screen.
 *
 * @return int32_t
 */
int32_t TS_Init(void)
{
   /* TS_Init */
   TS_Init_t TS_InitStruct;
   TS_InitStruct.Width = TS_MAX_WIDTH;
   TS_InitStruct.Height = TS_MAX_HEIGHT;
   TS_InitStruct.Orientation = TS_SWAP_NONE;
   TS_InitStruct.Accuracy = TS_ACCURACY;

   int32_t ret = BSP_TS_Init(TS_INSTANCE, &TS_InitStruct);
   if (ret != BSP_ERROR_NONE)
      return ret;
   // ret = BSP_TS_EnableIT(TS_INSTANCE);
   return ret;
}

/**
 * @brief  Initializes the LCD screen with default settings.
 * @retval None
 */
void LCD_InitScreen(void)
{
   /* Set the LCD Context */
   Lcd_Ctx[0].ActiveLayer = 0;
   Lcd_Ctx[0].PixelFormat = LCD_PIXEL_FORMAT_ARGB8888;
   Lcd_Ctx[0].BppFactor = 4; /* 4 Bytes Per Pixel for ARGB8888 */
   Lcd_Ctx[0].XSize = 800;
   Lcd_Ctx[0].YSize = 480;
   /* Disable DSI Wrapper in order to access and configure the LTDC */
   __HAL_DSI_WRAPPER_DISABLE(&hlcd_dsi);

   /* Initialize LTDC layer 0 iused for Hint */
   LCD_LayertInit(0, LCD_FRAME_BUFFER);
   UTIL_LCD_SetFuncDriver(&LCD_UTIL_Driver);

   /* Enable DSI Wrapper so DSI IP will drive the LTDC */
   __HAL_DSI_WRAPPER_ENABLE(&hlcd_dsi);

   /* Clear display */
   UTIL_LCD_Clear(APP_COLOR_BACKGROUND);

   /*Refresh the LCD display*/
   HAL_DSI_Refresh(&hlcd_dsi);
}