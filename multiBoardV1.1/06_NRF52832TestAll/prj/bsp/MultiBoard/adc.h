#ifndef __ADC_H__
#define __ADC_H__

#include "stdint.h"
#include "nrf_drv_saadc.h"

void initLightAdc(uint8_t fu8Chnl);
void unInitLightAdc(void);

void initBatteryAdc(uint8_t fu8Chnl);
void unInitBatteryAdc(void);
#endif
