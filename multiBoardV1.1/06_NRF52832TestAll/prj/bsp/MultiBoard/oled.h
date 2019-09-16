#ifndef __OLED_H__
#define __OLED_H__

#include "nrf_drv_spi.h"
#include "boards.h"
#include "nrf_gpio.h"

#define LCD_CS_L					nrf_gpio_pin_clear(SPIM1_SS_PIN)
#define LCD_CS_H					nrf_gpio_pin_set(SPIM1_SS_PIN)

#define LCD_RST_L					nrf_gpio_pin_clear(LCD_RST_PIN)
#define LCD_RST_H					nrf_gpio_pin_set(LCD_RST_PIN)

#define LCD_DC_CMD					nrf_gpio_pin_clear(LCD_DC_PIN)
#define LCD_DC_DATA					nrf_gpio_pin_set(LCD_DC_PIN)

void initSpiToOled(void);
void closeOled(void);
void openOled(void);
void initOled(void);
void writeOledCmd(uint8_t fu8Cmd);
void writeOledData(uint8_t fu8Data);
void fillOledRam(uint8_t fu8ClrData);
void clrOledRam(void);
void clrOledLine( uint8_t fu8Yloc);
void writeOled8x16Number(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number);
void writeOled8x16Numbers(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum);
void writeOled8x16ASCII(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number);
void writeOled8x16ASCIIs(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum);
void writeOled5x7ASCII(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number);
void writeOled5x7ASCIIs(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum);
void writeOled12x24Number(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number, int8_t fu8OffSet);
void writeOled12x24NumberReserve(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number, int8_t fu8OffSet);
void clrOled12x24Number(uint8_t fu8Xloc, uint8_t fu8Yloc, int8_t fu8OffSet);
void writeOled12x24Numbers(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum);
void writeOled6x8Number(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number, int8_t fu8OffSet);
void writeOled6x8Numbers(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum);

void writeOled8x16TwoDot(uint8_t fu8Xloc, uint8_t fu8Yloc);
void clrOled8x16TwoDot(uint8_t fu8Xloc, uint8_t fu8Yloc);

void welcomScreen(void);
void writeOled5x7Arrow(uint8_t fu8Xloc, uint8_t fu8Yloc, bool bClear);

#endif
