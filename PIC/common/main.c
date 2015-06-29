#include <stdbool.h>
#include <common/common.h>
#include <CommonConstants.h>
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


bool GetMessageFromOutput(unsigned char messageType, MessageId command, unsigned char const *data, unsigned char count, unsigned char *value)
{
#if defined(HAVE_OUTPUT)
	bool retVal;

	INnOUT_PORT = 0;
#	if defined(HAVE_INPUT)
		INPUT_MESSAGE_MISSED = false;
#	endif
	I2cMasterInit();
	retVal = I2cMasterGet(messageType, command, value);
	I2cSlaveInit();
#	if defined(HAVE_INPUT)
		if (INPUT_MESSAGE_MISSED)
			g_inputMessageMissed = true;
#	endif
	INnOUT_PORT = 1;
	return retVal;
#else
	return false;
#endif
}

void SendStateToOutputIfReady()
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
	
	if (SendMessageToOutput(I2C_MESSAGE_TYPE_DATA, 0, &g_outState, 1))
		g_toOutput.isReady = false;
	else if (!INPUT_MESSAGE_MISSED)//message was not send
		InvertOutput();
}

void ProcessStateChangedCommon()
{
	g_stateChanged = false;

#ifdef HAVE_OUTPUT
	g_toOutput.send_try = TO_OUTPUT_MAX_TRAY;
	g_toOutput.isReady = true;
#endif
}

void main(void)
{
	unsigned stateRepeater = 0;
#ifndef INTERFACE
	INTCONbits.GIE = 0;

	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);

	TRISC = 0x0; //mainl for debug
	TRISA = 0x0; //mainly RA0 and RA1 should be as configured as outputs because the could cause external interupt which I use for Input bus checking
	
#	if defined(HAVE_INPUT) && defined(HAVE_OUTPUT)
		TRISAbits.TRISA2 = 1; //input for iterupt
		INPUT_MESSAGE_MISSED = false; //interupt flag cleared
		OPTION_REGbits.INTEDG = 0; //external interupt to falling edge
#	endif

	g_persistant.mode = 1;
	g_persistant.bootLoaderCheck = RUN_PROGRAM_VALUE;
	CommonInit();
#endif
	ModuleTypeSpecificInit();
	INVERT_OUTPUT_TRIS = false; //FIXME: it should not be here, it should be enough to set it in CommonInit, but it doesn work for the interface module
	while(1)
	{
#if defined(HAVE_INPUT)
#	if defined (HAVE_OUTPUT)
		//While I sended message to output predecessor try to send a message to me. I have to wait for it again
		if (g_inputMessageMissed)
		{
			unsigned inputCounter = RECEIVE_TRY_COUNT; //at least one question to input message
			g_inputMessageMissed = false;
			while (0 != inputCounter-- && !SSP1IF)
			{};
		}
#	endif
		CheckI2cAsSlave();
		
		INTF = 0; //i2c message has benn processed, clear input interrupt flag

		if (g_commandRecieved)
		{

			ProcessCommandModuleTypeSpecific();
			ProcessCommandCommon();
		}
#endif

		ProcessModuleFunctionality();

		if (g_stateMessageEnabled && (g_stateChanged || stateRepeater++ == 1000))
		{
			stateRepeater = 0;
			ProcessStateChangedModuleTypeSpecific();
			ProcessStateChangedCommon();
		}
#if defined (HAVE_OUTPUT)
		SendStateToOutputIfReady();
#endif
	}
}