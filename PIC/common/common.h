#ifndef _COMMON_COMMON1_H_
#define _COMMON_COMMON1_H_

#include <stdbool.h>

#if defined(HAVE_INPUT) && defined(HAVE_OUTPUT)
#   define INPUT_MESSAGE_MISSED (INTF)
#else
#   define INPUT_MESSAGE_MISSED (false)
#endif

#define STATE_MAX 0xff
typedef enum
{
    I2C_MESSAGE_TYPE_DATA  = 0,
    I2C_MESSAGE_TYPE_COMMAND  = 1
} I2cMessageType;

void Wait(int delay);
void SwitchControllerInit();
void ProcessCommandCommon();
void ProcessStateChangedCommon();
void CommonInit();

#ifndef BOOTLOADER
struct
{
    unsigned char bootLoaderCheck;
    unsigned char mode;
} g_persistant;

unsigned char g_inState = STATE_MAX;
unsigned char g_outState = 0;

bool g_stateChanged = true;
bool g_commandRecieved = false;
unsigned char g_commandInstruction = 0;
unsigned char g_commandValue = 0;
struct
{
    bool isState:1;
    bool isReady:1;
    unsigned send_try:6;
} g_toOutput;
#endif //BOOTLOADER


#endif //_COMMON_COMMON1_H_