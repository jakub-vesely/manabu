#include <common/pwm.h>
#include <CommonConstants.h>

#define RED_TO_RED_PERIOD 85 //6 states
#define RED_TO_PURPLE_PERIOD 102 //5 states

#define RED_CALIBRATION 0.8
#define GREEN_CALIBRATION 1.2
#define BLUE_CALIBRATION 1

typedef enum
{
	MODE_RED_TO_RED = 1,
	MODE_RED_TO_PURPLE = 2,
	MODE_WHITE_VALUE = 3,
} MODE;

void ProcessCommandModuleTypeSpecific()
{
}
void SetDutyCyclePWM(unsigned char red, unsigned char green, unsigned char blue)
{
	SetDutyCyclePWM1((unsigned char)((float)green * GREEN_CALIBRATION));
	SetDutyCyclePWM2((unsigned char)((float)red * RED_CALIBRATION));
    SetDutyCyclePWM3((unsigned char)((float)blue * BLUE_CALIBRATION));
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
	g_outState = g_inState;
	switch (g_persistant.mode)
	{
		case MODE_RED_TO_RED:
			SetColor(g_outState/4, RED_TO_RED_PERIOD, true);
			break;
		case MODE_RED_TO_PURPLE:
			SetColor(g_outState/4, RED_TO_PURPLE_PERIOD, true);
			break;
		default: //white and initial value from HEFLASH after programming
			SetWhiteValue(g_outState/8);
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