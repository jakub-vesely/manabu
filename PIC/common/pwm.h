//inspired by http://www.microchip.com/forums/m677165.aspx

#ifndef _COMMON_PWM_H_
#define _COMMON_PWM_H_

#include <common/common.h>
#include <system_common.h>

void SetDutyCyclePWM1(unsigned x);
void SetDutyCyclePWM2(unsigned x);
void SetDutyCyclePWM3(unsigned x);

void PwmInit();

#endif //_COMMON_PWM_H_