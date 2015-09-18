//inspired by http://www.microchip.com/forums/m677165.aspx

#ifndef _COMMON_I2C_H_
#define _COMMON_I2C_H_

#include <common/common.h>
#include <system_common.h>
#include <CommonConstants.h>

void I2cSlaveInit();
void I2cMasterInit(void);
void I2cInit(void);
bool I2cMasterIdle(void);
bool PutStateI2C(unsigned char state);
bool PutCommandToI2C(MessageId command, unsigned char const *data, unsigned char count);
bool GetCommandFromI2C(MessageId command, unsigned char *retVal, unsigned char count);
bool CheckI2cAsSlave(void);
bool I2cMasterPut(unsigned char messageType, MessageId command, unsigned char const *data, unsigned char count);
bool I2cMasterGet(unsigned char messageType, MessageId command, unsigned char *retVal, unsigned char count);


#define SCL_PORT LATC0
#define SDA_PORT LATC1
#define SCL_TRIS TRISC0
#define SDA_TRIS TRISC1

#define  I2C_COMMON_INIT\
    SSPEN = 0;\
    SCL_PORT = 0;\
    SDA_PORT = 0;\
    SCL_TRIS = 1;\
    SDA_TRIS = 1;

#define I2C_SLAVE_SPECIFIC_INIT \
    SSPCON2 = 0b00000001; /*SEN is set to enable clock stretching*/\
    SSPCON3 = 0x00; \
    SSPMSK = 0x00; /*all address bits will be ignored*/\
    SSPADD = 0x00; /*no address is set, all the time is conneceted only one slave*/\
    SSPSTAT = 0x00; \
    SSPCON1 = 0b00010110; /*clock stretching + 7-bit addressing*/ \
    SSPEN = 1;  /*enable SSP and configures SDA & SCL pins*/

#endif //_COMMON_I2C_H_
