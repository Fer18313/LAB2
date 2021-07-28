#ifndef __8bits_LCD_h_
#define	__8bits_LCD_h_

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#define _XTAL_FREQ 4000000
#define RS RE0
#define EN RE1

void initLCD(void);
void wsLCD(char *x);
void wLCD(char x);
void LCD_set(char x);
void LCD_cursor(char x, char y);
void LCDsetup(char x);
#endif	/* XC_HEADER_TEMPLATE_H */