#ifndef _COMMON_COMMON1_H_
#define _COMMON_COMMON1_H_

typedef enum
{
    COMMAND_PING = 0,
    COMMAND_CHANGE_MODE = 1,
    COMMAND_GET_CURRENT_MODE = 2,
    COMMAND_FLASH_START = 3, //used to get version too
    COMMAND_FLASH_END = 4,
    COMMAND_FLASH_ADDRESS = 5,
    COMMAND_FLASH_LATCH_WORD = 6,
    COMMAND_FLASH_WRITE_WORD = 7,
    COMMAND_FLASH_CHECKSUM = 8
    
} I2cCommand;

typedef enum
{
    I2C_MESSAGE_TYPE_DATA  = 0,
    I2C_MESSAGE_TYPE_COMMAND  = 1
} I2cMessageType;

#endif //_COMMON_COMMON1_H_