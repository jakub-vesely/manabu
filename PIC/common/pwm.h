//inspired by http://www.microchip.com/forums/m677165.aspx

#ifndef _COMMON_PWM_H_
#define _COMMON_PWM_H_

#include <common/common.h>

volatile unsigned char PWM1DCL @ 0x611;
volatile unsigned char PWM1DCH @ 0x612;
volatile unsigned char PWM1CON @ 0x613;

volatile bit PWM1EN @ ((unsigned)&PWM1CON*8)+7;
volatile bit PWM1OE @ ((unsigned)&PWM1CON*8)+6;
volatile bit PWM1OUT @ ((unsigned)&PWM1CON*8)+5;
volatile bit PWM1POL @ ((unsigned)&PWM1CON*8)+4;


volatile unsigned char PWM2DCL @ 0x614;
volatile unsigned char PWM2DCH @ 0x615;
volatile unsigned char PWM2CON @ 0x616;

volatile bit PWM2EN @ ((unsigned)&PWM2CON*8)+7;
volatile bit PWM2OE @ ((unsigned)&PWM2CON*8)+6;
volatile bit PWM2OUT @ ((unsigned)&PWM2CON*8)+5;
volatile bit PWM2POL @ ((unsigned)&PWM2CON*8)+4;

volatile unsigned char PWM3DCL @ 0x617;
volatile unsigned char PWM3DCH @ 0x618;
volatile unsigned char PWM3CON @ 0x619;

volatile bit PWM3EN @ ((unsigned)&PWM3CON*8)+7;
volatile bit PWM3OE @ ((unsigned)&PWM3CON*8)+6;
volatile bit PWM3OUT @ ((unsigned)&PWM3CON*8)+5;
volatile bit PWM3POL @ ((unsigned)&PWM3CON*8)+4;




#define SetDutyCyclePWM1(x,) (PWM1DCH = x)
#define SetDutyCyclePWM2(x) (PWM2DCH = x) 
#define SetDutyCyclePWM3(x) (PWM3DCH = x) 

void PwmInit();

#endif //_COMMON_PWM_H_