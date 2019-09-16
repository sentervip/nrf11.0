#ifndef __MULTIBOARD_PROCESS_H__
#define __MULTIBOARD_PROCESS_H__

#include "stdint.h"
#include "bsp.h"

typedef enum
{
	TEST_ITEM_START = 0,
	TEST_ITEM_MPU6050,
	TEST_ITEM_BEEP,
	TEST_ITEM_KEY,
	TEST_ITEM_LIGHT_ADC,
	TEST_ITEM_FLASH,
	TEST_ITEM_RGB,
	TEST_ITEM_MOTO,
	TEST_ITEM_BATTERY,
	TEST_ITEM_VERSION,
	TEST_ITEM_QUIT,
}TEST_ITEM_DEF;

typedef struct
{
	uint8_t 	u8MenuIndex;
	uint8_t 	u8MenuItemIndex;
	uint8_t		u8CurserIndex;
	uint8_t 	u8FirstMenuItem;
	bsp_event_t	tKeyEvent;
}MENU_VAL_DEF;

typedef struct
{
	uint8_t u8MenuIndex;
	uint8_t u8UpKeyActionIndex;
	uint8_t u8DownKeyActionIndex;
	uint8_t u8LeftKeyActionIndex;
	uint8_t u8RightKeyActionIndex;
	uint8_t u8SelectKeyActionIndex;
	void	(*pFunc)();
}MENU_PROCESS_DEF;

typedef struct
{
	uint8_t u8RDuty;
	uint8_t u8GDuty;
	uint8_t u8BDuty;
}RGB_DEF;

typedef struct
{
	MENU_VAL_DEF 	tMenuVal;
	uint32_t		u32BeepFreq;
	uint8_t			u8BeepDuty;
	uint32_t 		b1DispTime:1;
	RGB_DEF			tRgb;
}SYS_VAL_DEF;


extern SYS_VAL_DEF gtSysVal;

void keyMenuProcess(bsp_event_t event);

#endif
