/**
 * @file   charger.h
 * @author Wim Looman & Henry Jenkins
 * @date   20110609
 */

#ifndef charger_h
#define charger_h

#include "common.h"

typedef void (*charger_callback) (bool);
/**
 * Initializes the charger module.
 */
void charger_init();

/**
 * Reads whether the charger is plugged in.
 *
 * @return true for charger plugged in, other wise false
 */
bool charger_read();

/**
 * Sets the callback to trigger when the charger is plugged in.
 *
 * @param callback The callback to trigger, the param is whether the pin just went high or low.
 */
void charger_set_callback(charger_callback);

#endif
