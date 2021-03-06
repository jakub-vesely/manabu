#ifndef _COMMON_CONSTANTS_H_
#define _COMMON_CONSTANTS_H_

#define FIRMWARE_VERSION 1

#define MESSAGE_LENGTH_BYTE_COUNT 1

#define PROTOCOL_ID "CUBO"

#define INTERFACE_MODULE_ADDRESS  0

//function ids
typedef enum MessageId{
MID_GET_FIRMWARE_VERSION = 0,
MID_COMMAND_FLASH_GET_VERSION = 1,
MID_COMMAND_FLASH_END = 2,
MID_COMMAND_FLASH_ADDRESS = 3,
MID_COMMAND_FLASH_LATCH_WORD = 4,
MID_COMMAND_FLASH_WRITE_WORD = 5,
MID_COMMAND_FLASH_CHECKSUM = 6,
MID_COMMAND_FLASH_SET_BOOT_FLAG = 7,

MID_GET_STATE = 8,
MID_SET_STATE = 9,
MID_GET_MODULE_TYPE = 10,
MID_GET_MODE = 11,
MID_SET_MODE = 12,
MID_DECREASE_INCREASE_MODE = 13, //value=0 for decrease, value=1 for increase

MID_SPECIFIC_COMMAND_8 = 24,
MID_SPECIFIC_COMMAND_7 = 25,
MID_SPECIFIC_COMMAND_6 = 26,
MID_SPECIFIC_COMMAND_5 = 27,
MID_SPECIFIC_COMMAND_4 = 28,
MID_SPECIFIC_COMMAND_3 = 29,
MID_SPECIFIC_COMMAND_2 = 30,
MID_SPECIFIC_COMMAND_1 = 31,
//i2c command id must be less the 2^5 (32)

	MID_INTERFACE_FLASHING_START = 34,
MID_INTERFACE_FLASHING_STOP =35
} MessageId;

#define DECREASE_MODE  1
#define INCREASE_MODE  2

typedef enum
{
	TYPE_NONE = 0,
    TYPE_USB_INTERFACE = 1,
    TYPE_POTENTIOMETER = 2,
    TYPE_RGB_LED = 3,
    TYPE_BUTTON = 4,
    TYPE_ARITHMETIC_LOGIC = 5,
    TYPE_GENERATOR = 6,
} ModuleTypes;


#endif //_COMMON_CONSTANTS_H_
