#ifndef _COMMON_COMMON_16F1503_H_
#define _COMMON_COMMON_16F1503_H_

#include <stdbool.h>
#include <pic16f1503.h>

#define INVERT_OUTPUT_PORT RA4
#define INnOUT_PORT RA5

#define INVERT_OUTPUT_TRIS TRISA4
#define INnOUT_TRIS TRISA5

#define RUN_PROGRAM_VALUE 0
#define IS_DATA SSPSTATbits.D_nA
#define IS_READ  SSPSTATbits.R_nW

#define ENABLE_I2C_MASTER_MODE_bit SSPM3

#ifndef BOOTLOADER
struct
{
    unsigned char bootLoaderCheck;
    unsigned char mode;
} g_persistant;

unsigned char g_state = 0;
bool g_stateChanged = true;
bool g_commandRecieved = false;
unsigned char g_commandInstruction = 0;
unsigned char g_commandValue = 0;

struct
{
    bool isState:1;
    bool isReady:1;
    unsigned try:6;
} g_toOutput;
#endif //BOOTLOADER

#endif //_COMMON_COMMON_16F1503_H_