/* 
 * File:   configuration.h
 * Author: paul
 *
 * Created on 04 February 2018, 16:23
 */

// CONFIG
// Important to set chip to use internal oscillator
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config IOSCFS = 8MHZ    // Internal Oscillator Frequency Select bit (8 MHz)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled)

// Using XC compiler
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

// Must define the oscillator frequency (8MHz) to use functions like __delay_ms
// Can be found in data sheet under "Oscillator Configurations"
#define _XTAL_FREQ 8000000