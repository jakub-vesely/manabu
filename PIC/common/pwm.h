//inspired by http://www.microchip.com/forums/m677165.aspx

#ifndef _COMMON_PWM_H_
#define _COMMON_PWM_H_

#include <htc.h>

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




#define SetDutyCyclePWM1(x) (PWM1DCH = x) 
#define SetDutyCyclePWM2(x) (PWM2DCH = x) 
#define SetDutyCyclePWM3(x) (PWM3DCH = x) 

void PwmInit(unsigned char pwmPeriod) 
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
 
    PR2 = pwmPeriod; 
 
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

#endif //_COMMON_PWM_H_