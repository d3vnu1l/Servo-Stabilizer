/*
 *
 */

#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "Inits.h"
#include "Defines.h"
#include "USART_Header_File.h"
#include "MPU6050.h"
#include "I2Cdev.h"

#pragma config FOSC = INTOSC_EC //internal oscillator 
#pragma config PLLDIV = 2       //divide by 2 to give 4MHz to PLL 
#pragma config FCMEN = OFF      //no fail safe clock monitor 
#pragma config IESO = OFF       //oscillator switchover disabled 
#pragma config PWRT = ON        //oscillator power up timer enabled (release version only) 
#pragma config BOR = OFF        //hardware brown out reset 
#pragma config WDT = OFF        //watchdog timer disabled 
#pragma config MCLRE = ON       //MCLR pin enabled 
#pragma config LPT1OSC = ON     //timer1 low power operation 
#pragma config PBADEN = OFF     //portB 0to 4 digital - not analogue 
#pragma config LVP = OFF        //low voltage programming disabled 
#pragma config CCP2MX = OFF     //portc1 = CCP2 
#pragma config XINST = OFF      //do not allow PIC18 extended instructions 
#pragma config  STVREN = ON     //stack overflow will cause reset 

int main(void) {
    char data_in=0;
    int16_t ax, ay, az, gx, gy, gz;
    uint8_t devStatus=1, mpuIntStatus, dmpReady;
    uint16_t packetSize;
    OSCCON = 0x72; // internal oscillator, 8MHz
    
    LATCbits.LC2=1; // Turn on Status Led
    
    initIO();
    initBT();
    
    //initSTEER();
    
    // I2C (400 kHz)
    OpenI2C(MASTER, SLEW_OFF);
    SSPADD = 14;
    // MPU6050
    MPU6050(MPU6050_ADDRESS_AD0_LOW);
    printf("try MPU... ");
    MPU6050_initialize();
    __delay_ms(500);
    //enable pin interrupt here
    if(MPU6050_testConnection())
        printf("Got MPU!\r\n");
    printf("Begin DMP init\r\n");
    __delay_ms(500);
    //devStatus = MPU6050_dmpInitialize();
    if(devStatus == 0){
        printf("Successfully init DMP!\r\n");
        printf("Enabling DMP... ");
        MPU6050_setDMPEnabled(true);
        // attach interrupt
        mpuIntStatus = MPU6050_getIntStatus();
        printf("DMP ready! Waiting on interrupt\r\n");
        dmpReady = 1;
        packetSize = MPU6050_dmpGetFIFOPacketSize();
    } else {
        printf("DMP init failed\r\n");
    }
    
    while (1) {
        //MPU6050_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        //printf("a/g:\t%d\t%d\t%d\t%d\t%d\t%d\r\n", ax, ay, az, gx, gy, gz);
        //data_in=0;
        printf("alive\r\n");
        __delay_ms(500);
        LATCbits.LC2=1;
        __delay_ms(500);
        LATCbits.LC2=0;
    }
}


