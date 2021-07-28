/*
 * File:   adc.c
 * Author: Fernando Sandoval
 *
 * Created on July 21, 2021, 6:44 PM
 */


#include <xc.h>
#include <stdint.h>
#include "adc.h"
#define _XTAL_FREQ 4000000


void ADC_START() {
        ADCON1bits.ADFM = 0; 
        ADCON1bits.VCFG0 = 0; 
        ADCON1bits.VCFG1 = 0; 
        ADCON0bits.ADCS = 0b10; 
        ADCON0bits.CHS = 0;            
        ADCON0bits.ADON = 1;
        __delay_us(50); 
        ADCON0bits.GO = 1;
    return;
}
