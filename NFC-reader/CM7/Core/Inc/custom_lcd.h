/**
 * @file custom_lcd.h
 * @brief Header file for LCD initialization and configuration functions.
 * @author Petr Kucera (kucerp28@fel.cvut.cz)
 */

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

/* Screen resolution and timing parameters */
#define VSYNC 10 // Vertical synchronization pulse (typically 5–10)
#define VBP 20   // Vertical Back Porch (typically 8–20)
#define VFP 20   // Vertical Front Porch (typically 8–20)
#define VACT 480 // Active height of the display (do not change)

#define HSYNC 40 // Horizontal synchronization pulse (typically 10–40)
#define HBP 40   // Horizontal Back Porch (typically 20–40)
#define HFP 40   // Horizontal Front Porch (typically 20–40)
#define HACT 800 // Active width of the display (do not change)

/* Touchscreen settings */
#define TS_ACCURACY 2
#define TS_INSTANCE 0

/* Time unit */
#define SECOND 1000

/* Application colors */
#define APP_COLOR_BACKGROUND UTIL_LCD_COLOR_CUSTOM_Stone
#define APP_COLOR_RED UTIL_LCD_COLOR_RED
#define APP_COLOR_BLUE UTIL_LCD_COLOR_CUSTOM_Blue
#define APP_COLOR_TEXT UTIL_LCD_COLOR_WHITE
#define APP_COLOR_GREEN UTIL_LCD_COLOR_DARKGREEN
#define APP_COLOR_YELLOW UTIL_LCD_COLOR_CUSTOM_Yellow
#define APP_COLOR_STONE UTIL_LCD_COLOR_BLACK

/* Frame buffer address */
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

/* Touchscreen functions */
int32_t TS_Init(void);

extern const LCD_UTILS_Drv_t LCD_UTIL_Driver;

#endif // __CUSTOM_LCD_H
