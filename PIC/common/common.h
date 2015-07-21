#ifndef _COMMON_COMMON1_H_
#define _COMMON_COMMON1_H_

#include <stdbool.h>
#include <CommonConstants.h>

#if defined(HAVE_INPUT) && defined(HAVE_OUTPUT)
#   define INPUT_MESSAGE_MISSED (INTF)
#else
#   define INPUT_MESSAGE_MISSED (false)
#endif

#define STATE_MAX 0x3ff
#define TO_OUTPUT_MAX_TRAY 10

#if defined(HAVE_INPUT) && defined(HAVE_OUTPUT)
	bool g_inputMessageMissed = false;
#endif
        
typedef enum
{
    I2C_MESSAGE_TYPE_DATA  = 0,
    I2C_MESSAGE_TYPE_COMMAND  = 1
} I2cMessageType;

void Wait(int delay);
void SwitchControllerInit();
void SetMode(unsigned char mode);
void ProcessCommandCommon();
void InvertOutput();
bool SendMessageToOutput(unsigned char messageType, MessageId command, unsigned char const *data, unsigned char count);
bool SendCommand(MessageId command, unsigned char const * data, unsigned char count);
unsigned char GetFromI2C(MessageId command, unsigned char* data, unsigned char count);
void CommonInit();

#ifndef BOOTLOADER
struct
{
    unsigned char bootLoaderCheck;
    unsigned char mode;
} g_persistant;

#define STATE_REPEATER_MAX 10000

unsigned g_inState = STATE_MAX;
unsigned g_outState = 0;

bool g_stateMessageEnabled = true; //sometimes (when flash process starts) is state message senduing disabled
bool g_stateChanged = true;
bool g_commandRecieved = false;
unsigned char g_commandInstruction = 0;
unsigned char g_commandValue = 0;
unsigned g_stateRepeater = 0;


struct
{
    bool isReady:1;
    unsigned send_try:6;
} g_toOutput;

#endif //BOOTLOADER


#endif //_COMMON_COMMON1_H_