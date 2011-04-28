/**
 * @file   motor.h
 * @author Wim Looman
 * @date 20110424
 */

#ifndef motor_h
#define motor_h

#include "common.h"
#include <avr/io.h>

#define FORWARD 0
#define REVERSE 1
#define LEFT    2
#define RIGHT   4

typedef uint8_t motor_direction_t;
typedef uint8_t motor_speed_t;
typedef uint8_t motor_side_t;

struct _motor {
    motor_direction_t direction;
    motor_speed_t speed;
    motor_side_t side;
};

typedef struct _motor *motor_t;

/**
 * Initialises a motor struct with the provided values.
 *
 * @param motor The motor struct to initialise.
 */
void motor_init(motor_t motor, motor_side_t side);

/**
 * Sets the speed of the provided motor.
 *
 * @param motor The motor to set the speed of.
 * @param speed The speed to set the motor at, 0 for stopped to 255 for full
 *              speed.
 */
void motor_set_speed(motor_t motor, motor_speed_t speed);

/**
 * Sets the direction of the provided motor.
 *
 * @param motor     The motor the set the direction on.
 * @param direction The direction to set the motor to.
 */
void motor_set_direction(motor_t motor, motor_direction_t direction);

#endif
