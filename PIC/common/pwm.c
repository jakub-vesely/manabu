#include "pwm.h"
#include <system_common.h>
void PwmInit()
{
    TRISC5 = 1; //disconnect PWM1 output
    TRISC3 = 1; //PWM2
    TRISA2 = 1; //PWM3

    PWM1CON = 0x00;
    PWM2CON = 0x00;
    PWM3CON = 0x00;

	PWM1DCH = 0x00;
    PWM2DCH = 0x00;
    PWM3DCH = 0x00;

	PWM1DCL = 0x00;
    PWM2DCL = 0x00;
    PWM3DCL = 0x00;

    PR2 = 255;

	//timer
    TMR2IF = 0; //disable timer interupt
    T2CONbits.T2CKPS = 0b00; //1:1 prescaler
    TMR2ON = 1; //enable timer interupt

	//Enable PWM output pins
    PWM1EN = 1;
    PWM2EN = 1;
    PWM3EN = 1;

    //Wait for Timer 2 to overflow
    while (!TMR2IF)
		;

    TRISC5 = 0;     //connect PWM1 output
    TRISC3 = 0;     //PWM2
    TRISA2 = 0;     //PWM3

    //Enable PWM output
    PWM1OE = 1;
    PWM2OE = 1;
    PWM3OE = 1;

    //Set polarity of PWM output
    //0 is default, active high
    PWM1POL = 0;
    PWM2POL = 0;
    PWM3POL = 0;

    //Set Module output value bit
    //default is 0
    PWM1OUT = 0;
    PWM2OUT = 0;
    PWM3OUT = 0;

    SetDutyCyclePWM1(0);
    SetDutyCyclePWM2(0);
    SetDutyCyclePWM3(0);
}

void SetDutyCyclePWM1(unsigned x)
{
    PWM1DCH = x >> 2;
    PWM1DCL = (x & 3) << 6;
}

void SetDutyCyclePWM2(unsigned x)
{
    PWM2DCH = x >> 2;
    PWM2DCL = (x & 3) << 6;
}

void SetDutyCyclePWM3(unsigned x)
{
    PWM3DCH = x >> 2;
    PWM3DCL = (x & 3) << 6;
}