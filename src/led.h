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
 * Sets the numbers to display.
 *
 * @param left      The left-hand number to display.
 * @param right     The right-hand number to display.
 */
void led_display(uint8_t, uint8_t);

/**
 * Sets the number to display on the left.
 *
 * @param number    The number to display.
 */
void led_display_left(uint8_t);

/**
 * Sets the number to display on the right.
 *
 * @param number    The number to display.
 */
void led_display_right(uint8_t);

#endif
