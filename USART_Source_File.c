/* PIC18F4550 USART Source File 
 * http://www.electronicwings.com
 */

#include "USART_Header_File.h"

/*****************************USART Initialization*******************************/
void USART_Init(long baud_rate)
{
    float temp;
    TRISC6=0;                       /*Make Tx pin as output*/
    TRISC7=1;                       /*Make Rx pin as input*/
    temp=Baud_value;     
    SPBRG=(int)temp;                /*baud rate=9600, SPBRG = (F_CPU /(64*9600))-1*/
    TXSTA=0x20;                     /*Transmit Enable(TX) enable*/ 
    RCSTA=0x90;                     /*Receive Enable(RX) enable and serial port enable */
}
/******************TRANSMIT FUNCTION*****************************************/ 
void putch(unsigned char byte){
    char trash;
    
    // Error handling
    if(RCSTAbits.FERR) 
        trash = RCREG;
    if(RCSTAbits.OERR) {
        RCSTAbits.CREN=0; 
        RCSTAbits.CREN=1;
    }
    
    // wait the end of transmission
    while (TXSTA1bits.TRMT == 0);
    TXREG1 = byte; // send the new byte
    while(!TXIF)
        continue;
    TXIF=0;
}
/*******************RECEIVE FUNCTION*****************************************/
char USART_ReceiveChar()
{
    if(RCIF==1)                 /*wait for receive interrupt flag*/
        return(RCREG);                  /*receive data is stored in RCREG register and return to main program */
    else
        return 0;
}
