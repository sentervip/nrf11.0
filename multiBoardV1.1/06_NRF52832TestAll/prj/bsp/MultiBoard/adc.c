#include "adc.h"
#include "app_timer.h"
#include "oled.h"
#include "nrf_delay.h"

APP_TIMER_DEF(gtLightSensorSampleTimer);
APP_TIMER_DEF(gtBatteryAdcSampleTimer);

void adcConvertCb(nrf_drv_saadc_evt_t const *evt)
{
	//sprintf((char *)lau8DispArry, "%d", evt->data);
	//writeOled5x7ASCIIs(0, 2, lau8DispArry);
}

void lightSensorSampleTimerHandle(void *p_context)
{
	int32_t ls32AdcResult = 0;
	nrf_saadc_value_t ls16AdcTmp = 0;
	for(uint8_t i = 0; i < 100; i ++)
	{
		nrf_drv_saadc_sample_convert(NRF_SAADC_INPUT_AIN3, &ls16AdcTmp);
		ls32AdcResult += ls16AdcTmp;
	}
	uint8_t lau8DispArry[16] = {0};
	clrOledLine(4);
	sprintf((char *)lau8DispArry, "%-4d", ls32AdcResult / 100);
	writeOled5x7ASCIIs(0, 4, lau8DispArry);
}


void initLightAdc(uint8_t fu8Chnl)
{
	nrf_drv_saadc_init(NULL, adcConvertCb);
	nrf_saadc_channel_config_t ltAdcChnl = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(fu8Chnl);
	nrf_drv_saadc_channel_init(fu8Chnl, &ltAdcChnl);

	app_timer_create(&gtLightSensorSampleTimer, APP_TIMER_MODE_REPEATED, lightSensorSampleTimerHandle);
	app_timer_start(gtLightSensorSampleTimer, APP_TIMER_TICKS(100, 0), NULL);
}

void unInitLightAdc(void)
{
	nrf_drv_saadc_uninit();
	app_timer_stop(gtLightSensorSampleTimer);
}

void batteryAdcSampleTimerHandler(void *p_context)
{
	int32_t ls32AdcResult = 0;
	nrf_saadc_value_t ls16AdcTmp = 0;
	nrf_gpio_pin_set(BAT_LEVEL_ENABLE_PIN);
	nrf_delay_ms(10);
	for(uint8_t i = 0; i < 100; i ++)
	{
		nrf_drv_saadc_sample_convert(NRF_SAADC_INPUT_AIN0, &ls16AdcTmp);
		ls32AdcResult += ls16AdcTmp;
	}
	nrf_gpio_pin_clear(BAT_LEVEL_ENABLE_PIN);
	uint8_t lau8DispArry[20] = {0};
	clrOledLine(4);
	double lfBatVolt = (double)ls32AdcResult * 9.0 / 64000.0;
	sprintf((char *)lau8DispArry, "Battery volt:%1.2fV", lfBatVolt);
	writeOled5x7ASCIIs(0, 4, lau8DispArry);
}

void initBatteryAdc(uint8_t fu8Chnl)
{
	nrf_drv_saadc_init(NULL, adcConvertCb);
	nrf_saadc_channel_config_t ltAdcChnl = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(fu8Chnl);
	nrf_drv_saadc_channel_init(fu8Chnl, &ltAdcChnl);

	nrf_gpio_cfg_output(BAT_LEVEL_ENABLE_PIN);

	app_timer_create(&gtBatteryAdcSampleTimer, APP_TIMER_MODE_REPEATED, batteryAdcSampleTimerHandler);
	app_timer_start(gtBatteryAdcSampleTimer, APP_TIMER_TICKS(2000, 0), NULL);
	batteryAdcSampleTimerHandler(NULL);
}

void unInitBatteryAdc(void)
{
	nrf_drv_saadc_uninit();
	app_timer_stop(gtBatteryAdcSampleTimer);
}
