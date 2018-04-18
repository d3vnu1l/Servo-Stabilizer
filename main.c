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
    float fax, fay, faz, fgz, fgy, fgx;
    float angle=180;
    float A = 0.962;
    float dt = 0.02;
    float error_angle;
    
    OSCCON = 0x72; // internal oscillator, 8MHz
    
    LATCbits.LATC2=1;
    
    initIO();
    initBT();
    //initSTEER();
    __delay_ms(30);
    // I2C (400 kHz)
    OpenI2C(MASTER, SLEW_OFF);
    SSPADD = 14;
    // MPU6050
    MPU6050(MPU6050_ADDRESS_AD0_LOW);
    printf("Init MPU... \r\n");
    MPU6050_initialize();
    __delay_ms(30);
    if(MPU6050_testConnection())
        printf("MPU enabled!\r\n");
    else printf("MPU failure\r\n");
    
    while (1) {
        MPU6050_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        //printf("a/g:\t%d\t%d\t%d\t%d\t%d\t%d\r\n", ax, ay, az, gx, gy, gz);
        
        angle = angle + gx*dt;
        error_angle = ax - angle;
        angle = angle + (1-A)*error_angle;
        
        
        printf("%f\r\n", angle);
        
        if(angle>360) angle-=360;
        __delay_ms(10);
        LATCbits.LC2=1;
        __delay_ms(10);
        LATCbits.LC2=0;
    }
}


