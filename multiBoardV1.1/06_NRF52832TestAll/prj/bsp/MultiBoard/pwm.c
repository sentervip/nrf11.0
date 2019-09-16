#include "pwm.h"
#include "nrf_drv_pwm.h"
#include "multiboard.h"
#include "bsp.h"
#include "app_util_platform.h"

nrf_drv_pwm_t gtPwm = NRF_DRV_PWM_INSTANCE(0);

void initBeepPwm(void)
{
	nrf_drv_pwm_config_t ltPwmCfg = NRF_DRV_PWM_DEFAULT_CONFIG(0);
	ltPwmCfg.output_pins[0] = BEEP_PWM_PIN;
	ltPwmCfg.output_pins[1] = NRF_DRV_PWM_PIN_NOT_USED;
	ltPwmCfg.output_pins[2] = NRF_DRV_PWM_PIN_NOT_USED;
	ltPwmCfg.output_pins[3] = NRF_DRV_PWM_PIN_NOT_USED;
	nrf_drv_pwm_init(&gtPwm, &ltPwmCfg, NULL);
}

void unInitBeepPwm(void)
{
	nrf_drv_pwm_stop(&gtPwm, true);
	nrf_drv_pwm_uninit(&gtPwm);
}

void setBeepPwmParams(uint32_t fu32Freq, uint8_t fu8Duty)
{
	static nrf_pwm_values_common_t ltSeqValues;
	uint32_t lu32TopValue = 1000000 / fu32Freq;
	ltSeqValues = lu32TopValue * fu8Duty / 100;;
	nrf_pwm_sequence_t ltSeq = 
	{
		.values.p_common 	= &ltSeqValues,
		.length 			= NRF_PWM_VALUES_LENGTH(ltSeqValues),
		.repeats			= 1,
		.end_delay			= 0,
	};
	gtPwm.p_registers->COUNTERTOP = lu32TopValue;
	nrf_drv_pwm_simple_playback(&gtPwm, &ltSeq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

static nrf_pwm_values_individual_t gtSeqValues = {0x8000 | 50, 0x8000 | 50, 0x8000 | 50, 0};
void initRgbPwm(void)
{
	nrf_drv_pwm_config_t ltPwmCfg = NRF_DRV_PWM_DEFAULT_CONFIG(0);
	ltPwmCfg.output_pins[0] = RGB_R_PIN;
	ltPwmCfg.output_pins[1] = RGB_G_PIN;
	ltPwmCfg.output_pins[2] = RGB_B_PIN;
	ltPwmCfg.output_pins[3] = NRF_DRV_PWM_PIN_NOT_USED;
	ltPwmCfg.load_mode = NRF_PWM_LOAD_INDIVIDUAL;
	nrf_drv_pwm_init(&gtPwm, &ltPwmCfg, NULL);

	nrf_pwm_sequence_t ltSeq = 
	{
		.values.p_common 	= (uint16_t *)&gtSeqValues,
		.length 			= NRF_PWM_VALUES_LENGTH(gtSeqValues),
		.repeats			= 1,
		.end_delay			= 0,
	};
	gtPwm.p_registers->COUNTERTOP = 100;
	nrf_drv_pwm_simple_playback(&gtPwm, &ltSeq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

void unInitRgbPwm(void)
{
	nrf_drv_pwm_stop(&gtPwm, true);
	nrf_drv_pwm_uninit(&gtPwm);
}

void setRPwmDuty(uint8_t fu8Duty)
{
	gtSeqValues.channel_0 = fu8Duty;
	nrf_pwm_sequence_t ltSeq = 
	{
		.values.p_common 	= (uint16_t *)&gtSeqValues,
		.length 			= NRF_PWM_VALUES_LENGTH(gtSeqValues),
		.repeats			= 1,
		.end_delay			= 0,
	};
	gtPwm.p_registers->COUNTERTOP = 100;
	nrf_drv_pwm_simple_playback(&gtPwm, &ltSeq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

void setGPwmDuty(uint8_t fu8Duty)
{
	gtSeqValues.channel_1 = fu8Duty;
	nrf_pwm_sequence_t ltSeq = 
	{
		.values.p_common 	= (uint16_t *)&gtSeqValues,
		.length 			= NRF_PWM_VALUES_LENGTH(gtSeqValues),
		.repeats			= 1,
		.end_delay			= 0,
	};
	gtPwm.p_registers->COUNTERTOP = 100;
	nrf_drv_pwm_simple_playback(&gtPwm, &ltSeq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

void setBPwmDuty(uint8_t fu8Duty)
{
	gtSeqValues.channel_2 = fu8Duty;
	nrf_pwm_sequence_t ltSeq = 
	{
		.values.p_common 	= (uint16_t *)&gtSeqValues,
		.length 			= NRF_PWM_VALUES_LENGTH(gtSeqValues),
		.repeats			= 1,
		.end_delay			= 0,
	};
	gtPwm.p_registers->COUNTERTOP = 100;
	nrf_drv_pwm_simple_playback(&gtPwm, &ltSeq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

