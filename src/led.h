/**
 * @file   led.h
 * @author Wim Looman
 * @date   20110530
 */

#ifndef led_h
#define led_h

#include "common.h"

/**
 * Initialises the led module.
 */
void led_init();

/**
 * Sets the number to display on the left.  If it's greater than 0x0F then the
 * left-hand display is turned off.
 *
 * @param number    The number to display.
 */
void led_display_left(uint8_t);

/**
 * Sets the number to display on the right.  If it's greater than 0x0F then the
 * right-hand display is turned off.
 *
 * @param number    The number to display.
 */
void led_display_right(uint8_t);

/**
 * Displays an up to 16-bit number across both LEDs.
 *
 * @param number    The number to display.
 */
static inline void led_display(uint8_t number) {
    led_display_left(number & 0x0F);
    led_display_right(number >> 4);
}

#endif
