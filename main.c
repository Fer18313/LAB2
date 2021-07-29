/*
 * File:   main.c
 * Author: Fernando Sandoval
 * CARNE: 18313
 * DIGITAL 2
 * Created on July 22, 2021, 2:32 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#define _XTAL_FREQ 4000000 

// LIBRARIES
#include <xc.h>
#include <stdint.h>
#include "ADC.h"
#include "UART.h"
#include "LCD8bits.h"

// VARS
  uint8_t temp_read; 
  uint8_t counter = 0; 
  uint8_t c;
  uint8_t var0; 
  uint8_t var1;
  uint8_t k1;
  uint8_t k0;
  uint8_t hundreds; 
  uint8_t unit; 
  uint8_t decimal;
  uint8_t decimal0;
  uint8_t unit_1;
  uint8_t t;
  uint8_t dozens;
  uint8_t unity;
  uint8_t F1; 
  uint8_t F0; 

//FUNCTION PROTOTYPES
void initCONFIG(void);  //Configuración
void configTMR0(void);
void LCD_display(void); //Subrutina para la LCD

// MAIN VOID
void main(void) {
    initCONFIG(); 
    configTMR0();
    while(1){     
    unit = var0 / 51;
    decimal = ((var0 * 100 / 51) - (unit*100))/10;
    decimal0 = ((var0 * 100 / 51) - (unit*100) - (decimal*10));  
    
    unit_1 = var1 / 51;
    k1 = (((var1 * 100) / 51) - (unit_1*100))/10;
    k0 = (((var1 * 100) / 51) - (unit_1*100) - (k1*10));
    hundreds = counter/100; 
    t = counter%100;
    dozens = t/10;
    unity = counter%10;
    
    if (decimal > 9){
        decimal = 9;
    }
    if (decimal0 > 9){
        decimal0 = 9;
    }
    if (k1 > 9){
        k1 = 9;
    }
    if (k0 > 9){
        k0 = 9;
    }
    
    if (unit > 5){
        unit = 5;
    }
    if (unit_1 > 5){
        unit = 5;
    }
    if (counter > 5){
        unit = 5;
    }
    // SETUP 1
    cursorLCD(2,1);
    wLCD(unit +48);
    cursorLCD(2,3);  
    wLCD(decimal + 48);
    cursorLCD(2,4);
    wLCD(decimal0 + 48);
    // SETUP 2
    cursorLCD(2,7);
    wLCD(unit_1 +48);
    cursorLCD(2,9);
    wLCD(k1 + 48);
    cursorLCD(2,10);
    wLCD(k0 + 48);
    // SETUP 3
    cursorLCD(2,13);
    wLCD(hundreds +48);
    cursorLCD(2,14);
    wLCD(dozens + 48);
    cursorLCD(2,15);
    wLCD(unity + 48);
    
    if(c > 20){ 
        c = 0; 
        TXIE = 1; 
    }
   } 
}

// INTERRUPT VOID

void __interrupt() isr(void){
    if (INTCONbits.T0IF){           
        c++;
        INTCONbits.T0IF = 0;        
    }
    if(ADIF == 1){
        switch (F0){  // SW FLAG TO CONTROL CHANNEL SELECTION OVER ADC CONVERSIONS
            case 1:
                var0 = ADRESH;
                ADCON0bits.CHS0 = 1;
                F0 = 0;
                break;
            case 0:
                var1 = ADRESH;
                ADCON0bits.CHS0 = 0;
                F0 = 1;
                break;
        }
                ADIF = 0; 
                __delay_us(120);
                ADCON0bits.GO = 1;   
    }
    if(PIR1bits.RCIF == 1){
        RB5 = 1;
        if (RCREG ==  0b1101){
        RB5 = 0;
            if (temp_read == 0b101110){
                counter++; 
                if (counter > 255){
                    counter = 0;
                } 
            }
            
            else if (temp_read == 0b101101){
                counter--;         
                if (counter > 255){
                    counter = 0;
                }
            }
        } 
        else {
        temp_read = RCREG;  
        }                 
    }
    if (TXIF == 1){
        switch(F1){
            case 0:
                TXREG = unit + 48;  // 48 MEANS ASCII FOR ZERO.
                F1 = 1;
                break;
            case 1:
                TXREG = 0b101110;
                F1 = 2;
                break;
            case 2:
                TXREG = decimal + 48; 
                F1 = 3;
                break;
            case 3:
                TXREG = decimal0 + 48; 
                F1 = 4;
                break;
            case 4:
                TXREG = 0b101101;
                F1 = 5;
                break;
            case 5:
                TXREG = unit_1 + 48;
                F1 = 6;    
                break;
            case 6:
                TXREG = 0b101110;
                F1 = 7;
                break;
            case 7:
                TXREG = k1 + 48;
                F1 = 8;
                break;
            case 8:
                TXREG = k0 + 48;
                F1 = 9;
                break;
            case 9:
                TXREG = 0b1101;
                F1 = 0;    
                break;
        }
        TXIF = 0;
    }
}

// CONFIGURATIONS VOID

void LCD_display(){
    cursorLCD(1,3);
    wsLCD("S1");
    cursorLCD(2,1);
    wsLCD("0.00");
    cursorLCD(1,8);
    wsLCD("S2");
    cursorLCD(2,7);
    wsLCD("0.00");
    cursorLCD(1,13);
    wsLCD("S3");
    cursorLCD(2,13);
    wsLCD("00");
    return;
}

void initCONFIG(void){
    ANSELH = 0;
    ANSEL = 0b00000011;  // AN0, AN1 SELECT CORRESPONDING TO RA0, RA1
    TRISA = 0b00000011;  // RA0, RA1 AS INPUTS
    TRISC = 0b11000000;  // TX, RX AS INPUTS CORRESPONDING TO RC6,RC7
    TRISD = 0; 
    TRISE = 0; 
    PORTA = 0; 
    PORTC = 0;    
    PORTD = 0;
    PORTE = 0;
    OSCCONbits.IRCF2 = 1; // OSCILLATOR TO 8MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    setupADC();          // ADC CONFIGURATION OVER @ ADC.C FILE 
    INTCONbits.GIE = 1;  
    INTCONbits.T0IE = 1;           
    INTCONbits.T0IF = 0; 
    INTCONbits.PEIE = 1; 
    PIE1bits.ADIE = 1;   
    PIR1bits.ADIF = 0; 
    initLCD();
    UARTs();
    LCD_display();
}

void configTMR0(){ // TMR0 CONFIGURATION CORRESPONDING TO APPROX. 5 MS
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1; 
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 217; 
}