#ifndef _COMMON_18F14K50_SYSTEM_COMMON_
#define _COMMON_18F14K50_SYSTEM_COMMON_

#include <stdbool.h>
#include <pic18F14K50.h>

#define FREQ 48000000

#define TRIS_SCL TRISBbits.TRISB6
#define TRIS_SDA TRISBbits.TRISB4

#define PORT_SCL PORTBbits.RB6
#define PORT_SDA PORTBbits.RB4 

#endif //_COMMON_18F14K50_SYSTEM_COMMON_