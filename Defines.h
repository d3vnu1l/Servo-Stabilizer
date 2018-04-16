
#ifndef DEFINES_H
#define	DEFINES_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define _XTAL_FREQ 8000000
#define USE_OR_MASKS

#define _BAUD_RATE 9600

#define MINTHR              8000
#define RESOLUTION          488

#define InternalOsc_8MHz    8000000
#define InternalOsc_4MHz    4000000
#define InternalOsc_2MHz    2000000
#define InternalOsc_1MHz    1000000
#define InternalOsc_500KHz  500000
#define InternalOsc_250KHz  250000
#define InternalOsc_125KHz  125000
#define InternalOsc_31KHz   31000

#define Timer2Prescale_1    1
#define Timer2Prescale_4    4
#define Timer2Prescale_16   16



#define LED LATCbits.LC2

#endif	/* XC_HEADER_TEMPLATE_H */

