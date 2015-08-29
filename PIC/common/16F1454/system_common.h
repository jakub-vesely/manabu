#ifndef _COMMON_16F1454_SYSTEM_COMMON_
#define _COMMON_16F1454_SYSTEM_COMMON_

#include <xc.h>
#include <stdbool.h>
#include <pic16f1454.h>


#define INVERT_OUTPUT_PORT LATA4
#define INnOUT_PORT LATA5

#define INVERT_OUTPUT_TRIS TRISA4
#define INnOUT_TRIS TRISA5

#define RUN_PROGRAM_VALUE 0
#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

#define ENABLE_I2C_MASTER_MODE_bit SSP1M3

#define FREQ 48000000
#endif //_COMMON_16F1454_SYSTEM_COMMON_