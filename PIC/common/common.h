#ifndef _COMMON_COMMON1_H_
#define _COMMON_COMMON1_H_

#include <stdbool.h>
#include <pic16f1503.h>

unsigned char g_value = 0;
bool g_valueChanged = true;
bool g_commandRecieved = false;
unsigned char g_commandInstruction = 0;
unsigned char g_commandValue = 0;

void CommonInit();
void Wait(int delay);

#endif //_COMMON_COMMON1_H_