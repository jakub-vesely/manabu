#include <stdbool.h>
#include <common/common.h>
#include <system_common.h>
#include <common/i2c.h>
#include <xc.h>
#include <moduleTypeSpecific.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#define RECEIVE_TRY_COUNT  5000

unsigned char g_invertOutput = 0;
bool g_inputMessageMissed = false;
bool SendMessageToOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count)
{
	bool retVal = 0;
	if (SSP1IF || SSPSTATbits.S) //message from input recieved or just comming
	{
		INPUT_MESSAGE_MISSED = false;
		return false;
	}
	
	SSPEN = 0;
	INnOUT_PORT = 0; //sitch to output
	INPUT_MESSAGE_MISSED = false; //it must be behind INnOUT_PORT = 0; because it cause iterupt

	I2cMasterInit();
	I2cMasterIdle();
	if (!INPUT_MESSAGE_MISSED)
		retVal = I2cMasterPut(messageType, command, data, count);
	I2cMasterIdle();
	SSPEN = 0;
	
	INnOUT_PORT = 1;
	I2cSlaveInit();
	if (INPUT_MESSAGE_MISSED)
		g_inputMessageMissed = true;
	
	return retVal;
}

bool GetMessageFromOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count, unsigned char *value)
{
	bool retVal;

	INnOUT_PORT = 0;
	INPUT_MESSAGE_MISSED = false;
	I2cMasterInit();
	retVal = I2cMasterGet(messageType, command, value);
	I2cSlaveInit();

	if (INPUT_MESSAGE_MISSED)
		g_inputMessageMissed = true;
	INnOUT_PORT = 1;
	return retVal;
}

void SendToOutputIfReady()
{
	if (!g_toOutput.isReady)
		return;

	if (0 == g_toOutput.send_try)
	{
		g_toOutput.isReady = false;
		//TODO: solve module on output is not connected any more
		return;
	}

	g_toOutput.send_try = g_toOutput.send_try - 1;
	
	if (g_toOutput.isState)
	{
		if (SendMessageToOutput(I2C_MESSAGE_TYPE_DATA, 0, &g_state, 1))
		{
			g_toOutput.isReady = false;
		}
		else if (!INPUT_MESSAGE_MISSED)//message was not send
		{
			if (g_invertOutput)
			{
				g_invertOutput = 0;
				INVERT_OUTPUT_PORT = 0;
			}
			else
			{
				g_invertOutput = 1;
				INVERT_OUTPUT_PORT = 1;
			}
		}
	}
	else
	{
		//TODO: command send
	}
}

void main(void)
{
	INTCONbits.GIE = 0;

	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);

	TRISA = 0x0; //mainly RA0 and RA1 should be as configured as outputs because the could cause external interupt which I use for Input bus checking
	TRISAbits.TRISA2 = 1; //input for iterupt

	INPUT_MESSAGE_MISSED = false; //interupt flag cleared
	OPTION_REGbits.INTEDG = 0; //external interupt to falling edge

	g_persistant.mode = 1;
	g_persistant.bootLoaderCheck = RUN_PROGRAM_VALUE;
	CommonInit();
	ModuleTypeSpecificInit();

	while(1)
	{
#if defined(HAVE_INPUT)
		
		//While I sended message to output predecessor try to send a message to me. I have to wait for it again
		if (g_inputMessageMissed)
		{
			unsigned inputCounter = RECEIVE_TRY_COUNT; //at least one question to input message
			g_inputMessageMissed = false;
			while (0 != inputCounter-- && !SSP1IF)
			{};
		}
		
		CheckI2cAsSlave();
		
		INTF = 0; //i2c message has benn processed, clear input interrupt flag

		if (g_commandRecieved)
			ProcessCommand();
#endif

		ProcessModuleFunctionality();

		if (g_stateChanged)
		{
			ProcessStateChangedModuleTypeSpecific();
			ProcessStateChangedCommon();
		}
#if defined (HAVE_OUTPUT)
		SendToOutputIfReady();
#endif
	}
}