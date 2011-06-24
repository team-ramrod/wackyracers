/**
 * @file   button.h
 * @author Wim Looman
 * @date   20110609
 */

#ifndef button_h
#define button_h

#include "common.h"

typedef enum {
    BUTTON_POWER
} button_t;

typedef void (*button_callback) (button_t);

/**
 * Initialises the button module.
 */
void button_init();

/**
 * Reads the value of the specified button.
 *
 * @param button The button to read.
 */
bool button_read(button_t);

/**
 * Sets a callback for when the specified button is pressed.
 *
 * @param button   The button to connect the callback to.
 * @param callback The callback to trigger when the button is hit, param is the
 *                 button pressed.
 *
 * WARNING:
 *   The callback is yet to be implemented.
 */
void button_set_callback(button_t, button_callback);

static inline void button_wait_power() {
    do {
    } while (button_read(BUTTON_POWER));
}

#endif
