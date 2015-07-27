#ifndef _COMMON_COMMON_H_
#define _COMMON_COMMON_H_

#include <stdbool.h>
#include <pic16f1503.h>

#define INVERT_OUTPUT_PORT LATA4
#define INnOUT_PORT LATA5

#define INVERT_OUTPUT_TRIS TRISA4
#define INnOUT_TRIS TRISA5

#define RUN_PROGRAM_VALUE 0
#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

#define ENABLE_I2C_MASTER_MODE_bit SSPM3

#define FREQ 16000000
#endif //_COMMON_COMMON_H_