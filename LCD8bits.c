/*
 * File:   LCD_8bit.c
 * Author: Fernando Sandoval
 *
 * Created on July 28, 2021, 4:54 PM
 */


#include <xc.h>
#include "LCD8bits.h"

// MODIFIED ACCORDING TO NEW NEEDS

void initLCD(){
   __delay_ms(80);
   LCDsetup(0b0);
   __delay_ms(110);
   enableLCD(0b110000);
   __delay_ms(40);
   enableLCD(0b110000);
   __delay_ms(390);
   enableLCD(0b110000);
   enableLCD(0b111000);
   enableLCD(0b10000);
   enableLCD(0b1);
   enableLCD(0b110);
   enableLCD(0b1100);
}
void enableLCD (char x){
    RS = 0;
    LCDsetup(x);
    EN = 1;
    __delay_ms(10);
    EN = 0;
}
void LCDsetup(char x) {
    PORTD = x;
}
void wLCD(char x){
    RS = 1;
    LCDsetup(x);
    EN = 1;
    __delay_us(100);
    EN = 0;
}
void cursorLCD(char x, char y){
    char t, u, v;
    if (x == 1){
        t = 0x80 + y - 1;
        enableLCD(t);
    }
    else if (x == 2){
        t = 0xC0 + y - 1;
        enableLCD(t);
    }
}
void wsLCD(char *x){
    int i;
    for(i=0;x[i]!='\0';i++)
        wLCD(x[i]);
}
