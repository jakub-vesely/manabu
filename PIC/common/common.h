#ifndef _COMMON_COMMON1_H_
#define _COMMON_COMMON1_H_

#include <stdbool.h>

#if defined(HAVE_INPUT) || defined(HAVE_OUTPUT)
#   define INPUT_MESSAGE_MISSED (INTF)
#else
#   define INPUT_MESSAGE_MISSED (false)
#endif

typedef enum
{
    COMMAND_NONE = 0,
    COMMAND_CHANGE_MODE = 1,
    COMMAND_GET_CURRENT_MODE = 2,
    COMMAND_FLASH_GET_VERSION = 3,
    COMMAND_FLASH_END = 4,
    COMMAND_FLASH_ADDRESS = 5,
    COMMAND_FLASH_LATCH_WORD = 6,
    COMMAND_FLASH_WRITE_WORD = 7,
    COMMAND_FLASH_CHECKSUM = 8,
    COMMAND_FLASH_SET_BOOT_FLAG = 9,
    COMMAND_PING = 10,
    COMMAND_GET_STATE = 11,
    COMMAND_GET_MODULE_TYPE = 12,
} I2cCommand;

typedef enum
{
    I2C_MESSAGE_TYPE_DATA  = 0,
    I2C_MESSAGE_TYPE_COMMAND  = 1
} I2cMessageType;

void Wait(int delay);
void SwitchControllerInit();
void ProcessCommand();
void ProcessStateChangedCommon();
void CommonInit();

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
    unsigned send_try:6;
} g_toOutput;
#endif //BOOTLOADER


#endif //_COMMON_COMMON1_H_