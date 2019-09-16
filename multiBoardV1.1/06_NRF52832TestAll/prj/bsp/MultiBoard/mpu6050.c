#include "mpu6050.h"
#include "nrf_drv_twi.h"
#include "app_util_platform.h"
#include "oled.h"
#include "app_timer.h"
#include "stdio.h"
#include "string.h"

nrf_drv_twi_t gtMpuTwi = NRF_DRV_TWI_INSTANCE(0);
APP_TIMER_DEF(gtMpuReadTimer);

void readMpuTimerHandler(void * fpContext);

void initMpuTwi(void)
{
	nrf_drv_twi_config_t ltMpuTwiCfg = NRF_DRV_TWI_DEFAULT_CONFIG(0);
	ltMpuTwiCfg.scl = MPU_TWI_SCL_PIN;
	ltMpuTwiCfg.sda = MPU_TWI_SDA_PIN;
	ltMpuTwiCfg.frequency = NRF_TWI_FREQ_100K;

	nrf_drv_twi_init(&gtMpuTwi, &ltMpuTwiCfg, NULL, NULL);
	nrf_drv_twi_enable(&gtMpuTwi);
}

void writeMpuReg(uint8_t fu8Add, uint8_t fu8Data)
{
	uint8_t lau8Data[2] = {0};
	lau8Data[0] = fu8Add;
	lau8Data[1] = fu8Data;

	nrf_drv_twi_tx(&gtMpuTwi, MPU6050_ADDRESS, lau8Data, 2, false);
}

uint8_t readMpuReg(uint8_t fu8Add)
{
	uint8_t lu8Data = {0};
	lu8Data = fu8Add;

	nrf_drv_twi_tx(&gtMpuTwi, MPU6050_ADDRESS, &lu8Data, 1, true);
	nrf_drv_twi_rx(&gtMpuTwi, MPU6050_ADDRESS, &lu8Data, 1);

	return lu8Data;
}

void readMultiMpuReg(uint8_t fu8Add, uint8_t* fau8Buff, uint8_t fu8Len)
{
	nrf_drv_twi_tx(&gtMpuTwi, MPU6050_ADDRESS, &fu8Add, 1, true);
	nrf_drv_twi_rx(&gtMpuTwi, MPU6050_ADDRESS, fau8Buff, fu8Len);
}

void initMpu(void)
{
	initMpuTwi();
	writeMpuReg(MPU6050_RA_PWR_MGMT_1, 0x00);
	writeMpuReg(MPU6050_RA_SMPLRT_DIV , 0x07);
	writeMpuReg(MPU6050_RA_CONFIG , 0x06);	
	writeMpuReg(MPU6050_RA_ACCEL_CONFIG , 0x01);
	writeMpuReg(MPU6050_RA_GYRO_CONFIG, 0x18); 

	app_timer_create(&gtMpuReadTimer, APP_TIMER_MODE_REPEATED, readMpuTimerHandler);
	app_timer_start(gtMpuReadTimer, APP_TIMER_TICKS(100, 0), NULL);
}

void uninitMpu(void)
{
	app_timer_stop(gtMpuReadTimer);
	nrf_drv_twi_uninit(&gtMpuTwi);
}

uint8_t readMpuId(void)
{
	return readMpuReg(MPU6050_RA_WHO_AM_I);
}

ACC_XYZ_DEF readMpuAcc(void)
{
	ACC_XYZ_DEF ltAcc = {0};
	int8_t las8Acc[6] = {0};

	readMultiMpuReg(MPU6050_ACC_OUT, (uint8_t *)las8Acc, 6);
	ltAcc.s16X = las8Acc[1] | (las8Acc[0] << 8);
	ltAcc.s16Y = las8Acc[2] | (las8Acc[3] << 8);
	ltAcc.s16Z = las8Acc[4] | (las8Acc[5] << 8);

	return ltAcc;
}

GYRO_XYZ_DEF readMpuGyro(void)
{
	GYRO_XYZ_DEF ltGyro = {0};
	int8_t las8Gyro[6] = {0};

	readMultiMpuReg(MPU6050_GYRO_OUT, (uint8_t *)las8Gyro, 6);
	ltGyro.s16X = las8Gyro[1] | (las8Gyro[0] << 8);
	ltGyro.s16Y = las8Gyro[2] | (las8Gyro[3] << 8);
	ltGyro.s16Z = las8Gyro[4] | (las8Gyro[5] << 8);

	return ltGyro;
}

int16_t readMpuTemp(void)
{
	int16_t ls16Temp = 0;
	int8_t las16Temp[2] = {0};

	readMultiMpuReg(MPU6050_RA_TEMP_OUT_H, (uint8_t *)las16Temp, 2);

	ls16Temp = las16Temp[1] | (las16Temp[0] << 8);

	return ls16Temp;
}

void displayMpuData(void)
{
	ACC_XYZ_DEF ltAcc = {0};
	GYRO_XYZ_DEF ltGyro = {0};
	int16_t ls16Temp = 0;

	ltAcc = readMpuAcc();
	ltGyro = readMpuGyro();
	ls16Temp = readMpuTemp();

	ls16Temp = ls16Temp / 34 + 365;

	uint8_t lau8DispBuff[6] = {0};
	uint8_t lu8StrLen = 0;
	
	lu8StrLen = sprintf((char *)lau8DispBuff, "%05d", ltAcc.s16X);
	lau8DispBuff[lu8StrLen] = '\0';
	writeOled5x7ASCIIs(0x00, 0x01, lau8DispBuff);
	
	memset(lau8DispBuff, 0, 6);
	lu8StrLen = sprintf((char *)lau8DispBuff, "%05d", ltAcc.s16Y);
	lau8DispBuff[lu8StrLen] = '\0';
	writeOled5x7ASCIIs(0x08, 0x01, lau8DispBuff);

	memset(lau8DispBuff, 0, 6);
	lu8StrLen = sprintf((char *)lau8DispBuff, "%05d", ltAcc.s16Z);
	lau8DispBuff[lu8StrLen] = '\0';
	writeOled5x7ASCIIs(0x00, 0x02, lau8DispBuff);

	memset(lau8DispBuff, 0, 6);
	lu8StrLen = sprintf((char *)lau8DispBuff, "%05d", ltGyro.s16X);
	lau8DispBuff[lu8StrLen] = '\0';
	writeOled5x7ASCIIs(0x00, 0x03, lau8DispBuff);

	memset(lau8DispBuff, 0, 6);
	lu8StrLen = sprintf((char *)lau8DispBuff, "%05d", ltGyro.s16Y);
	lau8DispBuff[lu8StrLen] = '\0';
	writeOled5x7ASCIIs(0x08, 0x03, lau8DispBuff);

	memset(lau8DispBuff, 0, 6);
	lu8StrLen = sprintf((char *)lau8DispBuff, "%05d", ltGyro.s16Z);
	lau8DispBuff[lu8StrLen] = '\0';
	writeOled5x7ASCIIs(0x00, 0x04, lau8DispBuff);

	memset(lau8DispBuff, 0, 6);
	lu8StrLen = sprintf((char *)lau8DispBuff, "%0d", ls16Temp);
	lau8DispBuff[lu8StrLen] = '\0';
	writeOled5x7ASCIIs(0x00, 0x05, lau8DispBuff);
}

void readMpuTimerHandler(void * fpContext)
{
	displayMpuData();
}

