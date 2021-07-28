/*
 * File:   newmain.c
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
#include "adc.h"
#include <pic16f887.h>

// VAR
uint8_t ADC_read0 = 0;
uint8_t ADC_read1 = 0;
uint8_t temp = 0;
uint8_t counter = 0;

uint8_t unit0;
uint8_t unit1;
uint8_t dec0;
uint8_t dec1;
uint8_t k0;
uint8_t k1;
uint8_t cents;
uint8_t temp1 = 0;
uint8_t dozen = 0;
uint8_t unity = 0;
uint8_t F = 0;

// FUNCTION PROTOTYPES
void initCONFIG(void);

// MAIN PROGRAM
void main(void) {
    initCONFIG();
    while (1){
        unit0 = ADC_read0/51;
        dec0 = ((ADC_read0*100/51)-(unit0*100));
        dec1 = ((ADC_read0*100/51)-(unit0*100) - (dec0*10));
        unit1 = ADC_read1/51;
        k0 = (((ADC_read1*100)/51) -(unit1*100))/10;
        k1  = (((ADC_read1*100)/51) - (unit1*100) - (k0*10));
        cents = counter/100;
        temp1 = counter%100;
        dozen = temp1/10;
        unity = counter%10;
        
        if (dec0>9){
            dec0 = 9;
        }
        if (dec1>9){
            dec1 = 9;
        }
        if (k0>9){
            k0 = 9;
        }
        if (k1>9){
            k1 = 9;
        }
        if (unit0>5){
            unit0=5;
        }
        if (unit1>5){
            unit0 = 5;
        }
        if (counter>5){
            unit0 =5;
        }
    }
    return;
}
void __interrupt() ISR(void){
        if(PIR1bits.ADIF){
            if (F ==1){
            ADC_read0 = ADRESH;
            ADCON0bits.CHS0 = 1;
            F = 0;
        }else{
            ADC_read1 = ADRESH;
            ADCON0bits.CHS0 = 0;
            F = 1;
            
}
        ADIF = 0;
        __delay_us(80);
        ADCON0bits.GO = 1;
}
        if(PIR1bits.RCIF ==1){
            RB5 = 1; // PORT F
         if (RCREG==0X0D) {
            RB5 = 0;
            if (temp =0x2B){
                counter++;
                if(counter >255){
                    counter=0;
                }
            }
            else if(temp==0x2D){
                counter--;
                if (counter>255){
                    counter =0;
                }
            }
        }
         else{
             temp = RCREG;
         }
}

}
void initLCD(){
    return;
}

void initCONFIG(){
    
    // OSCILLATOR CONFIG
    OSCCONbits.IRCF2 = 1; // 4MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;
    // CONFIG ADC
    ADCON1bits.ADFM=0;
    ADCON1bits.VCFG0=0;
    ADCON1bits.VCFG1=0;
    ADCON0bits.ADCS= 0b010;
    ADCON0bits.CHS = 0; // CHANNEL 0
    __delay_us(50); // ESPERAMOS UN TIEMPO PARA EL CAPACITOR
    ADCON0bits.ADON = 1;
    ADCON1bits.ADFM=0;
    ADCON1bits.VCFG0=0;
    ADCON1bits.VCFG1=0;
    ADCON0bits.CHS = 1; // CHANNEL 1
    __delay_us(50); // ESPERAMOS UN TIEMPO PARA EL CAPACITOR
    ADCON0bits.ADON = 1;
    
    // MAIN INTERRUPTIONS
    INTCONbits.GIE = 1;
    INTCONbits.PEIE =1;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF =0;
    
    // ADC INTERRUPT 
    PIE1bits.ADIE=1;
    PIR1bits.ADIF=0;
    
    // PORT B INTERRUPT
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCB = 0b01100000;
    
    // PORT CONFIGURATIONS
    TRISA = 0b00000000; // READING: ADC // PORT: AN0/RA0
    TRISB = 0b00000000; // INPUT PUSHBUTTON: RB5> UP , RB6> DOWN
    TRISC = 0;          // LED OUTPUT 
    TRISD = 0;          // 7 SEGMENT OUTPUT
    TRISE = 0;          // OUTPUT FOR SIGNAL CHANGE RE0 > UNITS | RE1 > CENTS
                        // RE2 > ALARM
    PORTA = 0;          
    PORTB = 0b00000000;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;          
    ANSELH = 0;
    ANSEL = 0b00000011; // ACTIVAMOS EL PUERTO ANALOGICO 0 y 1
    
    // PULL UP CONFIGURATION FOR PORTB 
    OPTION_REGbits.nRBPU = 0; // PARA FACILITARNOS HAREMOS USO DE LA FUNCIONALIDAD DE PULL UP DEL PIC EN EL PUERTO B
    WPUB = 0b00000000;
    return;
}
