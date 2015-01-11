#ifndef _COMMON_COMMON_H_
#define _COMMON_COMMON_H_

#include <stdbool.h>
#include <common/common.h>

unsigned char g_value = 0;
bool g_valueChanged = true;

void CommonInit()
{
	OSCCONbits.IRCF = 0b1111; //16MHz
	while (!OSCSTATbits.HFIOFS);
}

void Wait(int delay)
{
	int i;
	int j;
	for (i = 0; i < 100 * delay; i++)
		for (j = 0; j < 50; j++);

}

#endif //_COMMON_COMMON_H_