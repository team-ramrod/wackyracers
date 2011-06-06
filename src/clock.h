/**
 * @file   clock.h
 * @author Wim Looman
 * @date   20110606
 */

#ifndef clock_h
#define clock_h

#include <avr/io.h>

/**
 * Sets the clock to use the 32Mhz internal oscillator.
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
    OSC.CTRL = 0x01;
}

#endif
