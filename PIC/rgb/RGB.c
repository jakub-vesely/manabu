#include <stdbool.h>
#include <common/common.h>
#include <common/i2c.h>
#include <common/pwm.h>
#include "HEFlash.h"

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

#define I2C_ADDRESS 0x01

void SetDutyCyclePWM(unsigned char red, unsigned char green, unsigned char blue)
{
	SetDutyCyclePWM1(green);
	SetDutyCyclePWM2(red);	          
    SetDutyCyclePWM3(blue);
}

void SetColor(unsigned char i, unsigned char pwmPeriod, bool blackAndWhite)
{
	unsigned value = i % pwmPeriod;
	if (i == 0)
	{
		if (blackAndWhite)
		{
			SetDutyCyclePWM(0, 0, 0);
			return;
		}
	}			
		
	if (i < pwmPeriod)
	{
       	
		SetDutyCyclePWM(pwmPeriod -1 - value, value, 0);          
		return;
	}
	
	if (i < pwmPeriod * 2) 
	{
		SetDutyCyclePWM(0, pwmPeriod -1 - value, value);
		return;
	}
	
	if (i < 255 || !blackAndWhite)
	{
		if (!blackAndWhite && i == 255 && value == 0)
			value = pwmPeriod-1;

		SetDutyCyclePWM(value, 0, pwmPeriod -1 - value);
		return;
	}
	
	SetDutyCyclePWM(28, 28, 28);
}

void SetWhiteValue(unsigned char value)
{
	SetDutyCyclePWM(value, value, value);
}

void SetRed(unsigned char pwmPeriod)
{
	SetDutyCyclePWM(pwmPeriod -1, 0, 0);          
}

void SetGreen(unsigned char pwmPeriod)
{
	SetDutyCyclePWM(0, pwmPeriod, 0);
}

void SetBlue(unsigned char pwmPeriod)
{
	SetDutyCyclePWM(0, 0, pwmPeriod -1);
}

void interrupt isr(void)
{
	SSP1IF = 0; //Clear interrupt flag
	unsigned char status = (SSPSTAT & 0b00101101);    //Mask out unimportant bits
	ProcessI2cInterrupt(status);
}

void main(void)
{
        CommonInit();
    
	ANSELA = 0x00;      //set analog pins to digital 
        ANSELC = 0x00;
        //TRISbits for PWM are set in InitPWM() 
    
	//unsigned char pwmPeriod = 85; //red to red
	unsigned char pwmPeriod = 102; //red to purple
        
	PwmInit(pwmPeriod);
	I2cInit(I2C_ADDRESS); 

	//SetRed(102);

	g_value = HEFLASH_readByte (0, 0);
	unsigned newValue = g_value+ 5;
	HEFLASH_writeBlock(0, (void*)&newValue, sizeof(newValue));
	while(1)
	{
		if (g_valueChanged)
		{
			SetColor(g_value, pwmPeriod, true);
		}
	}
}

