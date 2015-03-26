#include <common/pwm.h>

#define COMMON_ANODE 0

#define RED_TO_RED_PERIOD 85 //6 states
#define RED_TO_PURPLE_PERIOD 102 //5 states

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

void ProcessStateChangedModuleTypeSpecific()
{
	g_mode = 1;
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

void ModuleTypeSpecificInit()
{
	PwmInit();
}

void ProcessModuleFunctionalit()
{
}