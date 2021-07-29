

#ifndef __LCD8bits_H
#define __LCD8bits_H
#include <xc.h>
#define _XTAL_FREQ 4000000
#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif


void initLCD(void);
void enableLCD(char x);
void LCDsetup(char x);
void wLCD (char x);
void cursorLCD (char x, char y);
void wsLCD (char *x);
#endif