/**
 * @file   chassis.h
 * @author Wim Looman
 */

#ifndef CHASSIS_H
#define CHASSIS_H

typedef enum {
    LEFT  = 0,
    RIGHT = 1
} chassis_t;

typedef enum {
    FORWARD = 2,
    REVERSE = 4
} chassis_direction_t;

typedef int8_t chassis_speed_t;

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
void chassis_set_speed(chassis_speed_t);

/**
 * Sets the steering direction.
 *
 * @param direction The direction to turn the chassis.
 */
bool chassis_set_direction(chassis_direction_t);

#endif
