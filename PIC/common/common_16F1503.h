#ifndef _COMMON_COMMON_16F1503_H_
#define _COMMON_COMMON_16F1503_H_

#include <stdbool.h>
#include <pic16f1503.h>
#include <common/i2c.h>

#define INPUT_SWITCH PORTAbits.RA5
#define OUTPUT_SWITCH PORTAbits.RA4

void Common16F1503Init();
void Wait(int delay);
void SwitchControllerInit();
void SendMessageToOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);
unsigned char GetMessageFromOutput(unsigned char messageType, I2cCommand command, unsigned char const *data, unsigned char count);
void ProcessCommand();
void ProcessStateChangedCommon();

unsigned char g_mode = 0;
unsigned char g_state = 0;
unsigned char g_stateChanged = true;
bool g_commandRecieved = false;
unsigned char g_commandInstruction = 0;
unsigned char g_commandValue = 0;

#endif //_COMMON_COMMON_16F1503_H_