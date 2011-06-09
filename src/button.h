/**
 * @file   button.h
 * @author Wim Looman
 * @date   20110609
 */

#ifndef button_h
#define button_h

#include "common.h"

typedef struct button *button_t;

typedef void (*button_callback) (button_t);

typedef struct {
    PORT_t *port;
    uint8_t out_pin;
    uint8_t in_pin;
} button_config;

/**
 * Initialises the button module.
 */
button_t button_init(button_config);

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
 */
void button_set_callback(button_t, button_callback);

#endif
