/*
 * File:   Inits.c
 * Author: ryan
 *
 * Created on March 28, 2018, 8:59 PM
 */


#include <xc.h>
#include "Inits.h"
#include "Defines.h"
#include "USART_Header_File.h"

void initIO(void){ 
    TRISA = 0;          // all output (except RA7 not implemented) 
    TRISB = 0x07;       // i2c as input
    TRISC1 = 1;         // Servo steer pwm in
    TRISC2 = 0;         // Status LED (PWM1)
    LATA=LATB=LATC=LATD=LATE=0x00;
}

void initBT(void){
    USART_Init(_BAUD_RATE);  /* initialize USART operation with 9600 baud rate */ 
    __delay_ms(50);
}

void initSTEER(void){
    TRISC2 = 0;   /* Set CCP1 pin as output for PWM out */
    CCP1CON = 0x0C;         /* Set PWM mode */
}
