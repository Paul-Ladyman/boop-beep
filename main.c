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

int thresh0 = 75;
int thresh1 = 50;
int thresh2 = 200;

void initMain() {
    // Set ports as digital I/O, not analogue input
    ANSEL = 0b00000000;

    /*
     * TRISC is the data direction register for PORTC.
     * Here we set all pins to be outputs (0)
     */
    TRISC = 0;
}

void main(void) {
    initMain();

    int count = 0;
    int rising = 1;

    while (1) {
        if (rising == 1) {
            count++;
        } else {
            count--;
        }

        if (count == 300) {
            rising = 0;
        } else if (count == 0) {
            rising = 1;
        }

        if (count > thresh0) {
            LED0 = 1;
        } else {
            LED0 = 0;
        }

        if (count > thresh1) {
            LED1 = 1;
        } else {
            LED1 = 0;
        }

        if (count > thresh2) {
            LED2 = 1;
        } else {
            LED2 = 0;
        }
    }
}