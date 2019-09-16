#include "multiboard_process.h"
#include "oled.h"
#include "utc_date.h"
#include "mpu6050.h"
#include "pwm.h"
#include "adc.h"
#include "flash.h"
#include "nrf_soc.h"
#include "string.h"
#include "nrf_delay.h"

#define MENU_LEVEL_1			1
#define MENU_LEVEL_2			2
#define MENU_LEVEL_3			3
#define MENU_LEVEL_4			4


SYS_VAL_DEF gtSysVal = {0};

#define MAX_MENU_NUM			(sizeof(gau8MenuChar) / sizeof(gau8MenuChar[0]))
#define                                                                             MENU_NUM_IN_SCREEN		7
const uint8_t gau8MenuChar[][10] = 
{
	"MPU6050",
	"Beep",
	"Key",
	"Light ADC",
	"Flash",
	"RGB",
	"Moto",
	"Battery",
	"Version",
	"Quit",
};

void clrMenuView(void)
{
	for(uint8_t i = 0; i < 7; i ++){
		clrOledLine(i + 1);
	}
}

void displayMainMenu(void)
{
	clrOledRam();
	gtSysVal.b1DispTime = true;
	displayTime();
	gtSysVal.tMenuVal.u8CurserIndex = 1;
	gtSysVal.tMenuVal.u8MenuItemIndex = 1;
	gtSysVal.tMenuVal.u8MenuIndex = 1;
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
	for(uint8_t i = 0; i < MENU_NUM_IN_SCREEN; i ++){
		if((gtSysVal.tMenuVal.u8FirstMenuItem + i) < MAX_MENU_NUM){
			writeOled5x7ASCIIs(1, i + 1, (uint8_t *)&gau8MenuChar[gtSysVal.tMenuVal.u8FirstMenuItem + i]);
		}
	}
}

void updateMainMenu(void)
{
	clrMenuView();
	displayTime();
	for(uint8_t i = 0; i < MENU_NUM_IN_SCREEN; i ++){
		if((gtSysVal.tMenuVal.u8FirstMenuItem + i) < MAX_MENU_NUM){
			writeOled5x7ASCIIs(1, i + 1, (uint8_t *)&gau8MenuChar[gtSysVal.tMenuVal.u8FirstMenuItem + i]);
		}
	}
}

void mainMenuUp()
{	
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, true);
	//printf("%d, %d\r\n", gtSysVal.tMenuVal.u8CurserIndex, gtSysVal.tMenuVal.u8MenuItemIndex);
	if((gtSysVal.tMenuVal.u8CurserIndex == 1) && (gtSysVal.tMenuVal.u8MenuItemIndex != 1)){
		if(gtSysVal.tMenuVal.u8CurserIndex){
			gtSysVal.tMenuVal.u8FirstMenuItem --;
			updateMainMenu();
		}
	}else{
		gtSysVal.tMenuVal.u8CurserIndex --;
		if(gtSysVal.tMenuVal.u8CurserIndex == 0){
			gtSysVal.tMenuVal.u8CurserIndex = MENU_NUM_IN_SCREEN;
		}
	}

	gtSysVal.tMenuVal.u8MenuItemIndex --;
	if(gtSysVal.tMenuVal.u8MenuItemIndex == 0){
		gtSysVal.tMenuVal.u8MenuItemIndex = MAX_MENU_NUM;
		gtSysVal.tMenuVal.u8FirstMenuItem = gtSysVal.tMenuVal.u8MenuItemIndex - MENU_NUM_IN_SCREEN;
		updateMainMenu();
	}

	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
}

void mainMenuDown()
{
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, true);

	gtSysVal.tMenuVal.u8MenuItemIndex ++;
	if(gtSysVal.tMenuVal.u8MenuItemIndex == MAX_MENU_NUM + 1){
		gtSysVal.tMenuVal.u8MenuItemIndex = 1;
		gtSysVal.tMenuVal.u8FirstMenuItem = 0;
		updateMainMenu();
	}
	if((gtSysVal.tMenuVal.u8MenuItemIndex - gtSysVal.tMenuVal.u8FirstMenuItem) <= MENU_NUM_IN_SCREEN){
		gtSysVal.tMenuVal.u8CurserIndex ++;
		if(gtSysVal.tMenuVal.u8CurserIndex == MENU_NUM_IN_SCREEN + 1){
			gtSysVal.tMenuVal.u8CurserIndex = 1;
		}
	}else{
		gtSysVal.tMenuVal.u8FirstMenuItem = gtSysVal.tMenuVal.u8MenuItemIndex - MENU_NUM_IN_SCREEN;
		updateMainMenu();
	}

	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
}

void showTestItem(void)
{
	clrMenuView();
	displayTime();
	switch(gtSysVal.tMenuVal.u8MenuItemIndex){
		case TEST_ITEM_MPU6050:
			initMpu();
			//gtSysVal.tMenuVal.u8MenuIndex = 3;
			break;
		case TEST_ITEM_BEEP:
			writeOled5x7ASCIIs(0, 2, "Beep Test!");
			initBeepPwm();
			setBeepPwmParams(1000, 50);
			writeOled5x7ASCIIs(0, 4, "Freq:1000Hz");
			writeOled5x7ASCIIs(0, 6, "Duty:50%");
			gtSysVal.u32BeepFreq = 1000;
			gtSysVal.u8BeepDuty = 50;
			break;
		case TEST_ITEM_KEY:
			writeOled5x7ASCIIs(0, 2, "Key Test:");
			break;
		case TEST_ITEM_LIGHT_ADC:
			writeOled5x7ASCIIs(0, 2, "Light ADC test:");
			initLightAdc(NRF_SAADC_INPUT_AIN3);
			break;
		case TEST_ITEM_FLASH:{
				uint8_t lau8Buff[20] = {0};
				uint8_t lau8FlashBuff[256] = {0};
				uint32_t lu32RandAdd = 0;
				uint8_t lu8RandLen = 0;
				uint16_t i;
				initFlash();
				sprintf((char *)lau8Buff, "Flash id is:%x", readFlashId());
				writeOled5x7ASCIIs(0, 2, lau8Buff);

				sd_rand_application_bytes_available_get(&lu8RandLen);
				if(lu8RandLen > 3)lu8RandLen = 3;
				sd_rand_application_vector_get((uint8_t *)&lu32RandAdd, lu8RandLen);
				sprintf((char *)lau8Buff, "Write to 0x%x:", lu32RandAdd & 0x00ffff00);
				writeOled5x7ASCIIs(0, 4, lau8Buff);
				writeOled5x7ASCIIs(0, 5, "0x00-0xFF");
				sprintf((char *)lau8Buff, "0x%x read check:", lu32RandAdd & 0x00ffff00);
				writeOled5x7ASCIIs(0, 6, lau8Buff);
				for(i = 0; i < 256; i ++){
					lau8FlashBuff[i] = i;
				}
				writeFlashPage(lau8FlashBuff, lu32RandAdd & 0x00ffff00);
				memset(lau8FlashBuff, 0, 256);
				readFlashPage(lau8FlashBuff, lu32RandAdd & 0x00ffff00);
				for(i = 0; i < 256; i ++){
					if(lau8FlashBuff[i] != i){
						writeOled5x7ASCIIs(0, 7, "Error");
						break;
					}
				}

				if(i == 256){
					writeOled5x7ASCIIs(0, 7, "OK");
				}
			}
			break;
		case TEST_ITEM_RGB:
			writeOled5x7ASCIIs(0, 2, "Press key down:");
			initRgbPwm();
			writeOled5x7ASCIIs(0, 4, "R:50 G:50 B:50");
			break;
		case TEST_ITEM_MOTO:
			writeOled5x7ASCIIs(0, 2, "Moto test:");
			nrf_gpio_cfg_output(MOTO_CNTRL_PIN);
			nrf_gpio_pin_set(MOTO_CNTRL_PIN);
			break;
		case TEST_ITEM_BATTERY:
			writeOled5x7ASCIIs(0, 2, "Battery test:");
			initBatteryAdc(NRF_SAADC_INPUT_AIN0);
			break;
		case TEST_ITEM_VERSION:
			writeOled5x7ASCIIs(0, 2, "V1.0");
			writeOled5x7ASCIIs(0, 3, __DATE__);
			writeOled5x7ASCIIs(0, 4, __TIME__);
			writeOled5x7ASCIIs(0, 5, "Support by rf_smart");
			writeOled5x7ASCIIs(0, 7, "rf-smart.taobao.com");
			break;
		case TEST_ITEM_QUIT:
			clrOledRam();
			NVIC_SystemReset();			
			break;
	}

	gtSysVal.tMenuVal.u8MenuIndex = 1 + 2 * gtSysVal.tMenuVal.u8MenuItemIndex;
}

void mainMenuProcess(void)
{
	switch(gtSysVal.tMenuVal.tKeyEvent){
		case BSP_EVENT_UP:
			mainMenuUp();
			break;
		case BSP_EVENT_DOWN:
			mainMenuDown();
			break;
		case BSP_EVENT_LEFT:
			break;
		case BSP_EVENT_RIGHT:
			break;
		case BSP_EVENT_SELECT:
			showTestItem();
			break;
	}
}

void mpuTestProcess(void)
{
	uninitMpu();
	updateMainMenu();
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
	gtSysVal.tMenuVal.u8MenuIndex = 1;
}

void beepTestProcess(void)
{
	uint8_t lau8Buff[20] = {0};
	switch(gtSysVal.tMenuVal.tKeyEvent){
		case BSP_EVENT_UP:
			gtSysVal.u32BeepFreq += 1000;
			if(gtSysVal.u32BeepFreq == 11000)gtSysVal.u32BeepFreq = 1000;
			break;
		case BSP_EVENT_DOWN:
			gtSysVal.u32BeepFreq -= 1000;
			if(gtSysVal.u32BeepFreq == 0)
			{
				gtSysVal.u32BeepFreq = 10000;
			}
			break;
		case BSP_EVENT_LEFT:
			gtSysVal.u8BeepDuty -= 10;
			if(gtSysVal.u8BeepDuty == 0){
				gtSysVal.u8BeepDuty = 100;
			}
			break;
		case BSP_EVENT_RIGHT:
			gtSysVal.u8BeepDuty += 10;
			if(gtSysVal.u8BeepDuty == 110)gtSysVal.u8BeepDuty = 10;
			break;
		case BSP_EVENT_SELECT:
			updateMainMenu();
			writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
			gtSysVal.tMenuVal.u8MenuIndex = 1;
			gtSysVal.u32BeepFreq = 1000;
			gtSysVal.u8BeepDuty = 50;
			unInitBeepPwm();
			break;
	}
	if(gtSysVal.tMenuVal.tKeyEvent != BSP_EVENT_SELECT)
	{
		clrOledLine(4);
		clrOledLine(6);
		sprintf((char *)lau8Buff, "Freq:%dHz", gtSysVal.u32BeepFreq);
		writeOled5x7ASCIIs(0, 4, lau8Buff);
		sprintf((char *)lau8Buff, "Duty:%d%", gtSysVal.u8BeepDuty);
		writeOled5x7ASCIIs(0, 6, lau8Buff);
		setBeepPwmParams(gtSysVal.u32BeepFreq, gtSysVal.u8BeepDuty);
	}
}

void keyTestProcess(void)
{
	clrOledLine(4);
	switch(gtSysVal.tMenuVal.tKeyEvent){
		case BSP_EVENT_UP:
			writeOled5x7ASCIIs(0, 4, "Key up Press!");
			break;
		case BSP_EVENT_DOWN:
			writeOled5x7ASCIIs(0, 4, "Key down Press!");
			break;
		case BSP_EVENT_LEFT:
			writeOled5x7ASCIIs(0, 4, "Key left Press!");
			break;
		case BSP_EVENT_RIGHT:
			writeOled5x7ASCIIs(0, 4, "Key right Press!");
			break;
		case BSP_EVENT_SELECT:
			updateMainMenu();
			writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
			gtSysVal.tMenuVal.u8MenuIndex = 1;
			break;
	}
}

void lightAdcTestProcess(void)
{
	updateMainMenu();
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
	gtSysVal.tMenuVal.u8MenuIndex = 1;
	unInitLightAdc();
}

void verisonProcess(void)
{
	updateMainMenu();
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
	gtSysVal.tMenuVal.u8MenuIndex = 1;
}

void flashAdcTestProcess(void)
{
	updateMainMenu();
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
	gtSysVal.tMenuVal.u8MenuIndex = 1;
	unInitFlash();
}

void rgbAdcTestProcess(void)
{
	switch(gtSysVal.tMenuVal.tKeyEvent){
		case BSP_EVENT_UP:

			break;
		case BSP_EVENT_DOWN:
			{
				uint8_t lau8Buff[20] = {0};
				uint32_t lu32Rand = 0;
				uint8_t lu8RandLen = 0;
				sd_rand_application_bytes_available_get(&lu8RandLen);
				if(lu8RandLen > 3)lu8RandLen = 3;
				sd_rand_application_vector_get((uint8_t *)&lu32Rand, lu8RandLen);
				setRPwmDuty((lu32Rand & 0XFF) % 100);
				setGPwmDuty(((lu32Rand >> 8) & 0XFF) % 100);
				setBPwmDuty(((lu32Rand >> 16)& 0XFF) % 100);
				clrOledLine(4);
				sprintf((char *)lau8Buff, "R:%d G:%d B:%d", (lu32Rand & 0XFF) % 100, ((lu32Rand >> 8) & 0XFF) % 100, ((lu32Rand >> 16)& 0XFF) % 100);
				writeOled5x7ASCIIs(0, 4, lau8Buff);
			}
			break;
		case BSP_EVENT_LEFT:

			break;
		case BSP_EVENT_RIGHT:
			break;
		case BSP_EVENT_SELECT:
			unInitRgbPwm();
			updateMainMenu();
			writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
			gtSysVal.tMenuVal.u8MenuIndex = 1;
			break;
	}
}

void motoAdcTestProcess(void)
{
	updateMainMenu();
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
	gtSysVal.tMenuVal.u8MenuIndex = 1;
	nrf_gpio_pin_clear(MOTO_CNTRL_PIN);
}

void batterycTestProcess(void)
{
	unInitBatteryAdc();
	updateMainMenu();
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
	gtSysVal.tMenuVal.u8MenuIndex = 1;
}

void quitProcess(void)
{
	updateMainMenu();
	writeOled5x7Arrow(0, gtSysVal.tMenuVal.u8CurserIndex, false);
	gtSysVal.tMenuVal.u8MenuIndex = 1;
}


MENU_PROCESS_DEF gtMenuProcess[] = 
{
//	menu index	
//	|  up button press
//	|  |  down button press
//	|  |  |	 left button press
//	|  |  |	 |	right button press
//	|  |  |	 |	|  select button press
//	|  |  |	 |	|  |	
	0, 0, 0, 0, 0, 1, NULL,
	1, 2, 2, 2, 2, 2, displayMainMenu,//main menu
	2, 0, 0, 0, 0, 0, mainMenuProcess,

	3, 0, 0, 0, 0, 4, NULL,//MPU6050 test
	4, 0, 0, 0, 0, 0, mpuTestProcess,

	5, 6, 6, 6, 6, 6, NULL,
	6, 0, 0, 0, 0, 0, beepTestProcess,

	7, 8, 8, 8, 8, 8, NULL,
	8, 0, 0, 0, 0, 0, keyTestProcess,

	9, 0, 0, 0, 0, 10, NULL,
	10, 0, 0, 0, 0, 0, lightAdcTestProcess,

	11, 0, 0, 0, 0, 12, NULL,
	12, 0, 0, 0, 0, 0, flashAdcTestProcess,

	13, 14, 14, 14, 14, 14, NULL,
	14, 0, 0, 0, 0, 0, rgbAdcTestProcess,

	15, 0, 0, 0, 0, 16, NULL,
	16, 0, 0, 0, 0, 0, motoAdcTestProcess,

	17, 0, 0, 0, 0, 18, NULL,
	18, 0, 0, 0, 0, 0, batterycTestProcess,

	19, 0, 0, 0, 0, 20, NULL,
	20, 0, 0, 0, 0, 0, verisonProcess,
};

void keyMenuProcess(bsp_event_t event)
{
	uint8_t lu8ActionIndex = 0;
	gtSysVal.tMenuVal.tKeyEvent = event;
	switch (event)
    {
		case BSP_EVENT_UP:
			lu8ActionIndex = gtMenuProcess[gtSysVal.tMenuVal.u8MenuIndex].u8UpKeyActionIndex;
			//printf("btn up press!\r\n");
			break;
		case BSP_EVENT_LEFT:
			lu8ActionIndex = gtMenuProcess[gtSysVal.tMenuVal.u8MenuIndex].u8LeftKeyActionIndex;
			//printf("btn left press!\r\n");
			break;
		case BSP_EVENT_RIGHT:
			lu8ActionIndex = gtMenuProcess[gtSysVal.tMenuVal.u8MenuIndex].u8RightKeyActionIndex;
			//printf("btn right press!\r\n");
			break;
		case BSP_EVENT_SELECT:
			lu8ActionIndex = gtMenuProcess[gtSysVal.tMenuVal.u8MenuIndex].u8SelectKeyActionIndex;
			//printf("btn select press!\r\n");
			break;
		case BSP_EVENT_DOWN:
			lu8ActionIndex = gtMenuProcess[gtSysVal.tMenuVal.u8MenuIndex].u8DownKeyActionIndex;
			//printf("btn down press!\r\n");
			break;		
        default:
            break;
    }
	
	if(gtMenuProcess[lu8ActionIndex].pFunc != NULL)
	{
		gtMenuProcess[lu8ActionIndex].pFunc();
	}
}

