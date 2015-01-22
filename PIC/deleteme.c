#include <xc.h> 
#include <stdint.h>        /* For uint8_t definition */ 
#include <stdbool.h>       /* For true/false definition */ 
#define I2C_ADDR 0x22 
#define PWM_PERIOD 0xFF 
#define SYS_FREQ    16000000L 
#define RXBUFFER_SIZE 4 
#define FCY         (SYS_FREQ/4) 
#define OSC_16MHZ   0b1111 
#define OSC_8MHZ    0b1110 
#define OSC_4MHZ    0b1101 
#define OSC_2MHZ    0b1100 
#define OSC_1MHZ    0b1011 
#define OSC_500KHZ  0b0111      //default upon reset 
#define OSC_250KHZ  0b0110 
#define TRIS_SCL TRISCbits.TRISC0 
#define TRIS_SDA TRISCbits.TRISC1 
///Sets 8 MSB's of duty cycle (8-bit resolution only) 
///setting the 2 MSB's of PWMxDCL would increase resolution to 10-bit 
#define SetDutyCyclePWM1(x) (PWM1DCH = x) 
#define SetDutyCyclePWM2(x) (PWM2DCH = x) 
#define SetDutyCyclePWM3(x) (PWM3DCH = x) 
#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = 0N, MCLRE = OFF, BOREN = 0FF, WRT = OFF, STVREN = 0FF, LVP = OFF 
    //Internal Oscillator 
    //Power-up timer enabled 
    //Watchdog timer disabled 
    //MCLR/Vpp pin function is digital input 
    //Brown-out reset disabled 
    //Write Protection off 
    //Stack Over/Underflow will not cause a reset 
    //Low voltage programmer disabled 
typedef enum _RGB_STATES 
{ 
    RGB_STATE_AUTO              = 0x00, 
    RGB_STATE_STATIC            = 0x01 
} RGB_STATES; 
//any variable modified by an ISR must be declared as volatile 
volatile unsigned char RXBuffer[RXBUFFER_SIZE], 
        RXBufferIndex = 0; 
unsigned char state;        //for AutoMode 
//Function Prototypes 
void I2CWrite(unsigned char); 
void AutoMode(); 
void InitPWM(void); 
void Initialize(void); 
void interrupt isr(void) 
{ 
    //This isr routine uses one if block as opposed to multiple ones. 
    //Using only one "if" avoids run time errors.  We handle only one interrupt at a time 
    //all variables that are modified by an ISR must be declared as volatile 
    /* Determine which flag generated the interrupt */ 
    if(SSP1IF)  //I2C interrupt handler 
    { 
        SSP1IF = 0;      //Clear interrupt flag 
        
        unsigned char i2cStatus, value; 
        i2cStatus = (SSPSTAT & 0b00101101);    //Mask out unimportant bits 
                        // D/A, S, R/W, BF 
        switch (i2cStatus){ 
            //State 1 RX address 
            //I2C write, last byte was an address 
            //SSPSTAT bits: D/A=0, S=1, R/W=0, BF=1 
            case 0b00001001: 
                value = SSPBUF;         //read buffer, clear BF 
                for(int i=0; i<RXBUFFER_SIZE;i++) 
                    RXBuffer[i] = 0;        //clear RXBuffer 
                RXBufferIndex = 0;          //clear index 
                if (SEN) CKP = 1;           //release the SCL line 
                break; 
            //State2 RX data 
            //I2C write, last byte was data 
            //SSPSTAT bits D/A=1, S=1, R/W=0, BF=1 
            case 0b00101001: 
                RXBuffer[RXBufferIndex] = SSPBUF; //read buffer, clear BF 
                RXBufferIndex++;                    //increment index 
                if (RXBufferIndex>=RXBUFFER_SIZE) 
                    RXBufferIndex = 0; 
                if (SEN) CKP = 1;           //release the SCL line 
                break; 
                //State 3 TX address 
                //I2C read, last byte was address 
                //SSPSTAT bits: D/A = 0, S=1, R/W=1, BF=0 
            case 0b00001100:    //possibly BF==1 
                value = SSPBUF;         //read buffer, clear BF 
                RXBufferIndex = 0; 
                I2CWrite(RXBuffer[value]);    //write back the index of status requested 
                RXBufferIndex++; 
                if (SEN) CKP = 1;           //release the SCL line 
                break; 
                //State 4 TX data 
                //I2C read, last byte was data 
                //SSPSTAT bits: D/A=1, S=1, R/W=1, BF=0 
            case 0b00101100: 
                if(RXBufferIndex>=RXBUFFER_SIZE) 
                    RXBufferIndex = 0; 
                I2CWrite(RXBuffer[value]);    //write back the index of status requested 
                RXBufferIndex++; 
                if (SEN) CKP = 1;           //release the SCL line 
                break; 
                //State 5 
                //Slave I2C logic reset by NACK from master 
                //SSPSTAT bits: D/A=1, S=1, R/W=0, BF=0, CKP=1 
            case 0b00101000: 
                break; 
            default: 
                break; 
        }//end switch (i2cStatus) 
    }//end I2C interrupt handler 
//    else if (interrupt #2) 
//    { 
//        clear interrupt #2 flag & handle interrupt 
//    } 
    else 
    { 
        /* Unhandled interrupts */ 
    } 
} 
void main(void){  
    OSCCONbits.IRCF = OSC_16MHZ;    // Configure the oscillator 
    while (!OSCSTATbits.HFIOFS);    //Wait for oscillator to stabilize 
    // Initialize I/O and Peripherals for application 
    Initialize(); 
    RXBuffer[0]=RGB_STATE_AUTO; 
    for (int i=1;i<=RXBUFFER_SIZE;i++) 
        RXBuffer[i] = 0; 
    
    while(1){        
        switch(RXBuffer[0]){ 
            case RGB_STATE_AUTO: 
                AutoMode(); 
                break; 
            case RGB_STATE_STATIC: 
                SetDutyCyclePWM1(RXBuffer[1]); 
                SetDutyCyclePWM2(RXBuffer[2]); 
                SetDutyCyclePWM3(RXBuffer[3]); 
                break; 
            default: 
                break; 
        }//switch(status[0]) 
    }//while 1 
   //never reach this 
} 
void I2CWrite(unsigned char data){ 
    while(BF);      //wait while buffer is full 
    do{ 
        WCOL = 0;           //clear write collision flag 
        SSP1BUF = data; 
    }while (WCOL);           //do until write collision flag is clear 
    if (SEN) CKP = 1;           //release the SCL line 
} 
void AutoMode(void){ 
    int i,j; 
    for (i=0;i<=255;i++){ 
        for (j=255;j>0;j--)NOP();//delay 
        switch (state){ 
            case 0://blue to magenta 
                SetDutyCyclePWM1(i/2); 
                SetDutyCyclePWM2(0); 
                SetDutyCyclePWM3(255-i/2); 
                break; 
            case 1://magenta to white 
                SetDutyCyclePWM1(128+i/2); 
                SetDutyCyclePWM2(i); 
                SetDutyCyclePWM3(128+i/2); 
                break; 
            case 2://white to red 
              //SetDutyCyclePWM1(255); 
                SetDutyCyclePWM2(255-i); 
                SetDutyCyclePWM3(255-i); 
                break; 
            case 3://red to yellow 
              //SetDutyCyclePWM1(255); 
                SetDutyCyclePWM2(i); 
              //SetDutyCyclePWM3(0); 
                break; 
            case 4://yellow to green 
                SetDutyCyclePWM1(255-i); 
                SetDutyCyclePWM2(255); 
                SetDutyCyclePWM3(0); 
                break; 
            case 5://green to cyan 
                SetDutyCyclePWM1(0); 
                SetDutyCyclePWM2(255); 
                SetDutyCyclePWM3(i); 
                break; 
            case 6://cyan to blue 
                SetDutyCyclePWM1(0); 
                SetDutyCyclePWM2(255-i); 
                SetDutyCyclePWM3(255); 
                break; 
            case 7://yellow to red 
                SetDutyCyclePWM1(128+i/2); 
                SetDutyCyclePWM2(i); 
                SetDutyCyclePWM3(128+i/2); 
                break; 
            case 8://red 
                SetDutyCyclePWM1(128+i/2); 
                SetDutyCyclePWM2(i); 
                SetDutyCyclePWM3(128+i/2); 
                break; 
            case 9://orange 
                SetDutyCyclePWM1(128+i/2); 
                SetDutyCyclePWM2(i); 
                SetDutyCyclePWM3(128+i/2); 
                break; 
            default://white 
                SetDutyCyclePWM1(i); 
                SetDutyCyclePWM2(i); 
                SetDutyCyclePWM3(i); 
                break; 
        }//switch state 
    }//for i 
    state++; 
    state%=7; 
} 
///Initialize PWM 1, 2 & 3 
void InitPWM(void) 
{ 
    //Set TRIS to disable PWM output 
    TRISC5 = 1;     //PWM1 
    TRISC3 = 1;     //PWM2 
    TRISA2 = 1;     //PWM3 
    //Clear PWMxCON register 
    PWM1CON = 0x00; 
    PWM2CON = 0x00; 
    PWM3CON = 0x00; 
    //Load the PR2 register with the PWM period value 
    PR2 = PWM_PERIOD; 
    //Clear PWMxDCH and PWMxDCL registers 
    PWM1DCH = 0x00; 
    PWM1DCL = 0x00; 
    PWM2DCH = 0x00; 
    PWM2DCL = 0x00; 
    PWM3DCH = 0x00; 
    PWM3DCL = 0x00; 
    //Configure and start Timer 2 
        //Clear Timer 2 interrupt flag 
        TMR2IF = 0; 
        //Configure Prescale Value 
        T2CONbits.T2CKPS = 0b00;    //1:1 prescaler 
        //Enable Timer 2 
        TMR2ON = 1; 
    //Enable PWM output pins 
    PWM1EN = 1; 
    PWM2EN = 1; 
    PWM3EN = 1; 
    //Wait for Timer 2 to overflow 
    while (!TMR2IF); 
    //Set PWM pins to output by clearing TRISbits 
    TRISC5 = 0;     //PWM1 
    TRISC3 = 0;     //PWM2 
    TRISA2 = 0;     //PWM3 
    //Enable PWM output 
    PWM1OE = 1; 
    PWM2OE = 1; 
    PWM3OE = 1; 
    //Set polarity of PWM output 
    //0 is default, active high 
    PWM1POL = 0; 
    PWM2POL = 0; 
    PWM3POL = 0; 
    //Set Module output value bit 
    //default is 0 
    PWM1OUT = 0; 
    PWM2OUT = 0; 
    PWM3OUT = 0; 
    SetDutyCyclePWM1(0); 
    SetDutyCyclePWM2(0); 
    SetDutyCyclePWM3(0); 
} 
void Initialize(void) 
{ 
    ANSELA = 0x00;      //set analog pins to digital 
    ANSELC = 0x00; 
        //TRISbits for PWM are set in InitPWM() 
    InitPWM(); 
    //Setup I2C----------------------------------------------------------------- 
    //Set I2C pins as inputs 
    TRIS_SCL = 1; 
    TRIS_SDA = 1; 
    SSP1CON2 = 0b00000001;  //SEN is set to enable clock stretching 
//      SEN = 1;            //Enable Clock Stretching 
    SSP1CON3 = 0x00; 
    SSP1ADD = I2C_ADDR<<1;      //7-bit address is stored in the 7 MSB's of the SSP1ADD register**********
    SSP1STAT = 0x00; 
    //SSP1CON1 bits with details 
    SSP1CON1 = 0b00110110; 
//      WCOL = 0;               //clear write collisions 
//      SSPOV = 0;              //clear receive overflow indicator 
//      SSPEN = 1;              //enable SSP and configures SDA & SCL pins 
//      CKP = 1;                //releases clock stretching 
//      SSPM = 0b0110;          //7-bit addressing slave mode 
//end I2C setup---------------------------------------------------------------- 
    /* Enable interrupts */ 
    SSP1IF  = 0;   //Clear MSSP interrupt flag 
    SSP1IE  = 1;   //I2C interrupt enable 
    PEIE    = 1;   // Enable Peripheral interrupts 
    GIE     = 1;   // Enable global interrupts 
} 