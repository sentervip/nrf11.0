#ifndef __PWM_H__
#define __PWM_H__

#include "stdint.h"


void initBeepPwm(void);
void unInitBeepPwm(void);
void setBeepPwmParams(uint32_t fu32Freq, uint8_t fu8Duty);

void initRgbPwm(void);
void unInitRgbPwm(void);
void setRPwmDuty(uint8_t fu8Duty);
void setGPwmDuty(uint8_t fu8Duty);
void setBPwmDuty(uint8_t fu8Duty);
#endif
