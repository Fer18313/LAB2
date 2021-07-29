/*
 * File:   ADC.c
 * Author: Fernando Sandoval
 *
 * Created on July 28, 2021, 4:54 PM
 */


#include <xc.h>
#include <stdint.h>
#include "ADC.h"

#define _XTAL_FREQ 4000000

void setupADC(void) {
    ADCON1bits.ADFM = 0; // SETUP ADCON1 // left justify/ vss/ vdd
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS = 0b10; // FOSC/32
    ADCON0bits.CHS = 0; // CHANNEL 0 SELECT BIT // AN0/RA0
    ADCON0bits.ADON = 1; // ADC ON
    __delay_us(100); 
    ADCON0bits.GO = 1; // START ADC CONVERSION
    return;
}
