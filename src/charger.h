/**
 * @file   charger.h
 * @author Wim Looman & Henry Jenkins
 * @date   20110609
 */

#ifndef charger_h
#define charger_h

#include "common.h"

/**
 * Initializes the charger module.
 */
void charger_init();

/**
 * Reads whether the charger is plugged in.
 */
bool charger_read();

/**
 * Sets the callback to trigger when the charger is plugged in.
 *
 * @param callback The callback to trigger
 */
void charger_set_callback(void (*)(void));

#endif
