/**
 * @file   dastardly.h
 *
 * Place for code to control dastardly hardware
 */

#ifndef dastardly_h
#define dastardly_h

#include "common.h"

/**
 * Initialises the dastardly module.
 */
void dastardly_init();

/**
 * Starts the ir leds
 */
void dastardly_ir_start();

/**
 * Stops the ir leds
 */
void dastardly_ir_stop();

#endif
