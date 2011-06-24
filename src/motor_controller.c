#include "motor_controller.h"
#include "motor.h"

#define FAST 30
#define SLOW 10
#define STOP 0

void motor_controller_init() {
    motor_init();
}

motor_direction_t __left_direction(motor_vert_t vert, motor_horiz_t horiz) {
    switch (vert) {
        case VERT_FORWARD:  return FORWARD;
        case VERT_BACKWARD: return REVERSE;
        case VERT_STOPPED:  break;
    }

    switch (horiz) {
        case HORIZ_LEFT: return REVERSE;
        default:         return FORWARD;
    }
}

motor_direction_t __right_direction(motor_vert_t vert, motor_horiz_t horiz) {
    switch (vert) {
        case VERT_FORWARD:  return FORWARD;
        case VERT_BACKWARD: return REVERSE;
        case VERT_STOPPED:  break;
    }

    switch (horiz) {
        case HORIZ_RIGHT: return REVERSE;
        default:          return FORWARD;
    }
}

motor_speed_t __left_speed(motor_vert_t vert, motor_horiz_t horiz) {
    switch (vert) {
        case VERT_STOPPED:
            return (horiz == HORIZ_STOPPED) ? STOP : FAST;
        case VERT_FORWARD:
        case VERT_BACKWARD:
            return (horiz == HORIZ_LEFT)    ? SLOW : FAST;
    }
    return STOP;
}

motor_speed_t __right_speed(motor_vert_t vert, motor_horiz_t horiz) {
    switch (vert) {
        case VERT_STOPPED:
            return (horiz == HORIZ_STOPPED) ? STOP : FAST;
        case VERT_FORWARD:
        case VERT_BACKWARD:
            return (horiz == HORIZ_RIGHT)   ? SLOW : FAST;
    }
    return STOP;
}

void motor_set_movement(motor_vert_t vert, motor_horiz_t horiz) {
    if ((vert + horiz) < 9) {
        motor_set_direction(LEFT,  __left_direction(vert, horiz));
        motor_set_direction(RIGHT, __right_direction(vert, horiz));
        motor_set_speed(LEFT,  __left_speed(vert, horiz));
        motor_set_speed(RIGHT, __right_speed(vert, horiz));
    } else {
        motor_set_direction(LEFT, FORWARD);
        motor_set_direction(RIGHT, FORWARD);
        motor_set_speed(LEFT, 0);
        motor_set_speed(RIGHT, 0);
    }
}
