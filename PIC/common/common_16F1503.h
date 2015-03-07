#ifndef _COMMON_COMMON_16F1503_H_
#define _COMMON_COMMON_16F1503_H_

#include <stdbool.h>
#include <pic16f1503.h>

void Common16F1503Init();
void Wait(int delay);

unsigned char g_mode = 0;

unsigned char g_state = 0;
bool g_stateChanged = true;
bool g_commandRecieved = false;
unsigned char g_commandInstruction = 0;
unsigned char g_commandValue = 0;

#endif //_COMMON_COMMON_16F1503_H_