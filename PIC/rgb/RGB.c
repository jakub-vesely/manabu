#include <stdbool.h>
#include <common/common.h>
#include <common/common_16F1503.h>
#include <common/i2c.h>
#include <common/pwm.h>
#include "HEFlash.h"

__CONFIG(FOSC_INTOSC & WDTE_OFF & MCLRE_OFF & BOREN_OFF & WRT_OFF & LVP_OFF &CP_OFF);

#define I2C_ADDRESS 0x02
#define RED_TO_RED_PERIOD 85 //6 states
#define RED_TO_PURPLE_PERIOD 102 //5 states

#define COMMON_ANODE 1

typedef enum
{
	MODE_RED_TO_RED = 1,
	MODE_RED_TO_PURPLE = 2,
	MODE_WHITE_VALUE = 3,
} MODE;

void SetDutyCyclePWM(unsigned char red, unsigned char green, unsigned char blue)
{
	SetDutyCyclePWM1(COMMON_ANODE ? 255 - green : green);
	SetDutyCyclePWM2(COMMON_ANODE ? 255 - red : red);
    SetDutyCyclePWM3(COMMON_ANODE ? 255 - blue : blue);
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
	ProcessI2cInterrupt();
}

void ProcessCommand()
{
	switch(g_commandInstruction)
	{
		case COMMAND_CHANGE_MODE:
			g_commandRecieved = false;
			g_mode = g_commandValue;
			HEFLASH_writeBlock(1, (void*)&g_mode, sizeof(g_mode)); //I dont understand why but radd = 0 doesn't work for me
			g_valueChanged = true;
			break;
	}
}

void SwitchControllerInit()
{
	LATA4 = 0;
	LATA5 = 0;
	TRISAbits.TRISA4 = 0;
	TRISAbits.TRISA5 = 0;
	PORTAbits.RA4 = 0;
	PORTAbits.RA5 = 1; //input connected
}
void main(void)
{
	CommonInit();
	SwitchControllerInit();
	PwmInit();
	g_mode = HEFLASH_readByte (1, 0);
	I2cInit(I2C_ADDRESS);

	while(1)
	{
		GCEN =1; //deleteme
		if (g_commandRecieved)
			ProcessCommand();

		if (g_valueChanged)
		{
			g_valueChanged = false;

			switch (g_mode)
			{
				case MODE_RED_TO_RED:
					SetColor(g_state, RED_TO_RED_PERIOD, true);
					break;
				case MODE_RED_TO_PURPLE:
					SetColor(g_state, RED_TO_PURPLE_PERIOD, true);
					break;
				default: //white and initial value from HEFLASH after programming
					SetWhiteValue(g_state);
					break;					
			}
		}
	}
}