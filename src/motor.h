/**
 * @file   motor.h
 * @author Wim Looman
 * @date   20110530
 */

#ifndef motor_h
#define motor_h

#include "common.h"

typedef enum {
    LEFT  = 0,
    RIGHT = 1
} motor_t;

typedef enum {
    FORWARD = 2,
    REVERSE = 4
} motor_direction_t;

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
 */
void motor_set_speed(motor_t, motor_speed_t);

/**
 * Sets the direction of the provided motor, also resets speed to 0 so
 * motor_set_speed should be called afterwards.  Non-reentrant, if called
 * while already executing will return false and nothing will be changed.
 *
 * @param motor     The motor to set the direction of.  @param direction The
 * direction to spin the motor.
 */
bool motor_set_direction(motor_t, motor_direction_t);

#endif
