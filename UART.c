/*
 * File:   UART.c
 * Author: Fernando Sandoval
 *
 * Created on July 28, 2021, 6:29 PM
 */
#include <xc.h>
#include <stdint.h>
#include "UART.h"

void UARTs() {    // ACCORDING TO THE OFFICIAL PIC 16F887 DATASHEET
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;  
    BAUDCTLbits.BRG16 = 1;  
    SPBRG = 207;
    SPBRGH = 0;   
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;   
    TXSTAbits.TXEN = 1;           
    return;
}