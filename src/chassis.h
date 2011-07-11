/**
 * @file   chassis.h
 * @author Wim Looman
 */

#ifndef CHASSIS_H
#define CHASSIS_H

#include "common.h"

typedef enum {
    FORWARD,
    REVERSE
} motor_direction_t;

typedef enum {
    LEFT,
    CENTRE,
    RIGHT
} chassis_direction_t;

typedef int8_t motor_speed_t;

/**
 * Initialises the chassis module.
 */
void chassis_init();

/**
 * Sets the speed of the chassis.
 *
 * @param speed     The speed to set the motor at, 0 for stopped to 127 for full
 *                  speed forwards to -128 to full speed reverse.
 */
void chassis_set_speed(motor_speed_t);

/**
 * Sets the steering direction.
 *
 * @param direction The direction to turn the chassis.
 */
void chassis_set_direction(chassis_direction_t);

#endif
