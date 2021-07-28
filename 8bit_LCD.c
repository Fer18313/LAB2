/*
 * File:   8bit_LCD.c
 * Author: Fernando Sandoval
 *
 * Created on July 25, 2021, 12:32 PM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "8bit_LCD.h"



void initLCD(){
    __delay_ms(50);
    LCDsetup(0x00);
    __delay_ms(50);
    LCDsetup(0x030);
    __delay_ms(50);
    LCDsetup(0x030);
    __delay_ms(200);
    LCDsetup(0x030);
    LCDsetup(0x010);
    LCDsetup(0x001);
    LCDsetup(0x006);
    LCDsetup(0x0C);  
}


void wsLCD(char *x){
    int i;
    for (i=0 ; x[i]!='\0' ; i++)
        wLCD(x[i]);
}
void wLCD(char x){
    RS = 1;
    LCDsetup(x);
    EN = 1;
    __delay_us(100);
    EN = 0;
}
void LCD_set(char x){
    RS = 0;
    LCDsetup(x);
    EN = 1;
    __delay_ms(80);
    EN = 0;
}
void LCDcursor(char x, char y){
    char a,b,c;
    if (x == 1){
        a = 0x80 + y - 1;
        LCD_set(a);
    }
    else if(x==2){
        a = 0xC0 + y - 1;
        LCD_set(a);
    }
}
void LCDsetup(char x) {
    PORTD = x;
}
