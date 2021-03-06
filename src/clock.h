/**
 * @file   clock.h
 * @author Wim Looman
 * @date   20110606
 */

#ifndef clock_h
#define clock_h

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * Sets the clock to use the 32Mhz internal oscillator.
 *
 * Main must call this before any other inits.
 */
static inline void clock_init() {
    // Enable the 32Mhz internal oscillator.
    OSC.CTRL |= _BV(1);

    // While the 32Mhz internal oscillator is not ready.
    while (!(OSC.STATUS & _BV(1))) {
        // Do nothing.
    }

    // Disable the configuration change protection.
    CCP = 0xD8;

    // Change to the 32Mhz internal oscillator.
    CLK.CTRL = 0x01;
}

/**
 * Enables all the interrupts.
 *
 * Main must call this after all other inits.
 */
static inline void interrupt_init() {
    sei();
    PMIC.CTRL |= 0x07;
}


#endif
