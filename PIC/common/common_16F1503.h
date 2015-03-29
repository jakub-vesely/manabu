#ifndef _COMMON_COMMON_16F1503_H_
#define _COMMON_COMMON_16F1503_H_

#include <stdbool.h>
#include <pic16f1503.h>

#define INVERT_OUTPUT RA4
#define INnOUT RA5
#ifdef FOUR_PIN_INTERFACE
#   define INPUT_SWITCH PORTAbits.RA5
#   define OUTPUT_SWITCH PORTAbits.RA4
#endif //FOUR_PIN_INTERFACE

void Common16F1503Init();
void Wait(int delay);
void SwitchControllerInit();
void ProcessCommand();
void ProcessStateChangedCommon();

unsigned char g_mode = 0;
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


#endif //_COMMON_COMMON_16F1503_H_