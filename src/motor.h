/**
 * @file   motor.h
 * @author Wim Looman
 * @date 20110424
 */

#ifndef motor_h
#define motor_h

#include "common.h"

typedef uint8_t direction_t;

#define FORWARD 1
#define REVERSE 0

typedef uint8_t speed_t;

struct _motor {
    direction_t direction;
    speed_t speed;
};

typedef struct _motor *motor_t;

/**
 * Initialises a motor struct with the provided values.
 *
 * @param motor The motor struct to initialise.
 */
void init_motor(motor_t motor);

/**
 * Sets the speed of the provided motor.
 *
 * @param motor The motor to set the speed of.
 * @param speed The speed to set the motor at, 0 for stopped to 255 for full speed.
 */
void set_motor_speed(motor_t motor, speed_t speed);

/**
 * Sets the direction of the provided motor.
 *
 * @param motor     The motor the set the direction on.
 * @param direction The direction to set the motor to.
 */
void set_motor_direction(motor_t motor, direction_t direction);

#endif
