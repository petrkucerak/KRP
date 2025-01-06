#ifndef __CUSTOM_LCD_H
#define __CUSTOM_LCD_H

#include "stm32_lcd.h"
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_bus.h"
#include "stm32h747i_discovery_conf.h"
#include "stm32h747i_discovery_lcd.h"
#include "stm32h747i_discovery_sdram.h"
#include "stm32h747i_discovery_ts.h"
#include "stm32h7xx_hal_dsi.h"
#include "stm32h7xx_hal_ltdc.h"

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

#define LCD_FRAME_BUFFER 0xD0000000

#define LAYER0_ADDRESS (LCD_FB_START_ADDRESS)

/* LCD screen functions */
uint8_t LCD_Init(void);
void LTDC_Init(void);

void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData,
                     uint16_t Size);
int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData,
                    uint16_t Size);
int32_t LCD_GetXSize(uint32_t Instance, uint32_t *XSize);
int32_t LCD_GetYSize(uint32_t Instance, uint32_t *YSize);
void LCD_MspInit(void);
void LCD_InitScreen(void);

/* TouchScreen functions */
int32_t TS_Init(void);

extern const LCD_UTILS_Drv_t LCD_UTIL_Driver;
#endif // __CUSTOM_LCD_H