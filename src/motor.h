/**
 * @file   motor.h
 * @author Wim Looman
 * @date 20110424
 */

#ifndef motor_h
#define motor_h

#include "common.h"
#include <avr/io.h>

typedef enum motor_direction_enum {
    FORWARD = 0,
    REVERSE = 1
} motor_direction_t;

typedef enum motor_enum {
    LEFT  = 2,
    RIGHT = 4
} motor_t;

typedef uint8_t motor_speed_t;

/**
 * Initialises the motor module.
 */
void motor_init();

/**
 * Sets the speed of the provided motor.
 *
 * @param motor     The motor to set the speed of.
 * @param speed     The speed to set the motor at, 0 for stopped to 255 for full
 *                  speed.
 * @param direction The direction to spin the motor.
 */
void motor_set_values(
        motor_t           motor,
        motor_direction_t direction,
        motor_speed_t     speed
);

#endif
