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

int pwmThresh1 = 99;
int pwmThresh2 = 85;

void initMain() {
    /*
     * Configure IO
     */
    // Set all ports as digital I/O, except AN0 which is set to analog input
    ANSEL = 0b00000001;
    // Set RA0 to be an input and all other PORTA pins to be outputs 
    TRISA = 0b00000001;
    // Set all PORTC pins to be outputs
    TRISC = 0;
    
    PORTC = 0b00000000;
    
    /*
     * Configure ADC
     */
    // Set conversion clock to Fosc/8
    ADCON1bits.ADCS = 0b001;
    // Set ADC results to be right justified (MSB in ADRESH)
    ADCON0bits.ADFM = 1;
    // Set Vref+ to Vdd
    ADCON0bits.VCFG = 0;
    // Set input channel to AN0
    ADCON0bits.CHS = 0b0000;
    // Zero ADC output variables
    ADRESL = 0;
    ADRESH = 0;

    /*
     * Configure Timer0
     */
    // Set to timer mode (internal clock source)
    OPTION_REGbits.T0CS = 0;
    // Assign Prescaler to TIMER0 instead of WatchDogTimer 
    OPTION_REGbits.PSA = 0;
    // Set Prescaler to 1
    OPTION_REGbits.PS = 0b000;
    // Time register preset
    TMR0 = 235;

    /*
     * Configure interrupt
     */
    // Clear ADC interrupt flag
    PIR1bits.ADIF = 0;
    // Enable ADC interrupts
    PIE1bits.ADIE = 1;
    // Enable peripheral interrupts
    INTCONbits.PEIE = 1;
    // enable Timer0 interrupt
    INTCONbits.T0IE = 1;
    // Enable global interrupts
    INTCONbits.GIE = 1;
}

void main(void) {
    initMain();

    while (1) {
        // enable ADC, begin acquiring input voltage
        ADCON0bits.ADON = 1;
        // wait for minimum acquisition time
        __delay_us(10);
        // start conversion of acquired voltage
        ADCON0bits.GO = 1;
    }
}

void interrupt ISR(void) {
    static int pwmThresh0 = 0;
    static int pwmBaseCount = 0;

    // if an ADC conversion result is ready
    if (PIE1bits.ADIE && PIR1bits.ADIF) {
        // Combine result MSB and LSB
        int result = ((ADRESH << 8) + ADRESL);
        
        pwmThresh0 = ((float)result / 1023.0) * 100.0;

        // Clear ADC interrupt flag
        PIR1bits.ADIF = 0;
    } else if (INTCONbits.T0IE && INTCONbits.T0IF) {
        // Reset timer flag
        INTCONbits.T0IF = 0;
        // Restore time register preset
        TMR0 = 235;

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
            // Set RC3 low
            PORTC &= ~(1 << 3);
        } else {
            // Set RC3 high
            PORTC |= (1 << 3);
        }
    }
}