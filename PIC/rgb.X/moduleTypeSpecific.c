#include <common/pwm.h>
#include <CommonConstants.h>

#define RED_TO_RED_PERIOD ((unsigned)((float)STATE_MAX / 3)) //6 states
#define RED_TO_PURPLE_PERIOD ((unsigned)((float)STATE_MAX / 2.5)) //5 states

#define RED_CALIBRATION 0.7
#define GREEN_CALIBRATION 1.0
#define BLUE_CALIBRATION 0.9

typedef enum
{
	MODE_RED_TO_PURPLE = 1,
    MODE_RED_TO_RED = 2,	
	MODE_WHITE_VALUE = 3,
} MODE;

void ProcessCommandModuleTypeSpecific()
{
}
void SetDutyCyclePWM(unsigned red, unsigned green, unsigned blue)
{
	SetDutyCyclePWM1((unsigned)((float)red * RED_CALIBRATION));
	SetDutyCyclePWM2((unsigned)((float)blue * BLUE_CALIBRATION));
    SetDutyCyclePWM3((unsigned)((float)green * GREEN_CALIBRATION));
}

void SetColor(unsigned i, unsigned pwmPeriod, bool blackAndWhite)
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

	if (i < STATE_MAX || !blackAndWhite)
	{
		if (!blackAndWhite && i == STATE_MAX && value == 0)
			value = pwmPeriod-1;

		SetDutyCyclePWM(value, 0, pwmPeriod -1 - value);
		return;
	}

	SetDutyCyclePWM(STATE_MAX/3,STATE_MAX/3,STATE_MAX/3);
}

void SetWhiteValue(unsigned value)
{
	SetDutyCyclePWM(value, value, value);
}

void SetRed(unsigned pwmPeriod)
{
	SetDutyCyclePWM(pwmPeriod -1, 0, 0);
}

void SetGreen(unsigned pwmPeriod)
{
	SetDutyCyclePWM(0, pwmPeriod, 0);
}

void SetBlue(unsigned pwmPeriod)
{
	SetDutyCyclePWM(0, 0, pwmPeriod -1);
}

void ProcessStateChangedModuleTypeSpecific()
{
	g_outState = g_inState;
	switch (g_persistant.mode)
	{
		case MODE_RED_TO_RED:
			SetColor(g_outState, RED_TO_RED_PERIOD, false);
			break;
		case MODE_RED_TO_PURPLE:
			SetColor(g_outState, RED_TO_PURPLE_PERIOD, true);
			break;
		default: //white and initial value from HEFLASH after programming
			SetWhiteValue(g_outState);
			break;
	}
}

void ModuleTypeSpecificInit()
{
	g_inState = 0; //it is not nice to led flash to max and after getting a state from predecessor is fading
	PwmInit();
}

void ProcessModuleFunctionality()
{
}

unsigned char GetModuleType()
{
	return TYPE_RGB_LED;
}