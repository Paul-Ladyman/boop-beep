/*
 * File:   main.c
 * Author: paul
 *
 * Created on 15 April 2018, 15:34
 */


#include "configuration.h"

// Define three pins from PortC to use as outputs for LEDs
#define LED0 RC3
#define LED1 RC4
#define LED2 RC5

int pwmThresh0 = 90;
int pwmThresh1 = 99;
int pwmThresh2 = 85;

void initMain() {
    // Set ports as digital I/O, not analogue input
    ANSEL = 0b00000000;

    /*
     * TRISC is the data direction register for PORTC.
     * Here we set all pins to be outputs (0)
     */
    TRISC = 0;

    // Set to timer mode (internal clock source)
    OPTION_REGbits.T0CS = 0;

    // Assign Prescaler to TIMER0 instead of WatchDogTimer 
    OPTION_REGbits.PSA = 0;
    // Set Prescaler to 16
    OPTION_REGbits.PS = 0b000;

    TMR0 = 180;

    // enable Timer0 interrupt
    INTCONbits.T0IE = 1;
    // Enable global interrupts
    INTCONbits.GIE = 1;
}

void main(void) {
    initMain();

    while (1);
}

void interrupt ISR(void) {
    static int pwmBaseCount = 0;
    // Reset timer flag
    INTCONbits.T0IF = 0;
    TMR0 = 180;

    /* 
     * Given 8MHz osc, 1:1 prescaler, 8-bit Timer (256) with 180 offset,
     * 100 counts to complete pwmBase signalcount
     * freq = 8MHz / 4 / 1 / 76 / 100 = 263Hz
     */
    if (pwmBaseCount < 100) {
        pwmBaseCount++;
    } else {
        pwmBaseCount = 0;
    }

    // LED turns on when pin is low
    if (pwmBaseCount > pwmThresh0) {
        LED0 = 0;
    } else {
        LED0 = 1;
    }

    if (pwmBaseCount > pwmThresh1) {
        LED1 = 0;
    } else {
        LED1 = 1;
    }

    if (pwmBaseCount > pwmThresh2) {
        LED2 = 0;
    } else {
        LED2 = 1;
    }
}