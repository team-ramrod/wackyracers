#include "motor_controller.h"
#include "motor.h"

#define NORMAL_SPEED 30
#define LOW_SPEED 10

void motor_controller_init() {
    motor_init();
}

void motor_set_movement(motor_vertical_t vert, motor_horizontal_t horiz) {
    switch(vert + horiz) {
        case VERTICAL_FORWARD + HORIZONTAL_STOPPED:
            motor_set_direction(LEFT, FORWARD);
            motor_set_direction(RIGHT, FORWARD);
            motor_set_speed(LEFT, NORMAL_SPEED);
            motor_set_speed(RIGHT, NORMAL_SPEED);
            break;
        case VERTICAL_BACKWARD + HORIZONTAL_STOPPED:
            motor_set_direction(LEFT, REVERSE);
            motor_set_direction(RIGHT, REVERSE);
            motor_set_speed(LEFT, NORMAL_SPEED);
            motor_set_speed(RIGHT, NORMAL_SPEED);
            break;
        case VERTICAL_STOPPED + HORIZONTAL_LEFT:
            motor_set_direction(LEFT, REVERSE);
            motor_set_direction(RIGHT, FORWARD);
            motor_set_speed(LEFT, NORMAL_SPEED);
            motor_set_speed(RIGHT, NORMAL_SPEED);
            break;
        case VERTICAL_FORWARD + HORIZONTAL_LEFT:
            motor_set_direction(LEFT, FORWARD);
            motor_set_direction(RIGHT, FORWARD);
            motor_set_speed(LEFT, LOW_SPEED);
            motor_set_speed(RIGHT, NORMAL_SPEED);
            break;
        case VERTICAL_BACKWARD + HORIZONTAL_LEFT:
            motor_set_direction(LEFT, REVERSE);
            motor_set_direction(RIGHT, REVERSE);
            motor_set_speed(LEFT, LOW_SPEED);
            motor_set_speed(RIGHT, NORMAL_SPEED);
            break;
        case VERTICAL_STOPPED + HORIZONTAL_RIGHT:
            motor_set_direction(LEFT, FORWARD);
            motor_set_direction(RIGHT, REVERSE);
            motor_set_speed(LEFT, NORMAL_SPEED);
            motor_set_speed(RIGHT, NORMAL_SPEED);
            break;
        case VERTICAL_FORWARD + HORIZONTAL_RIGHT:
            motor_set_direction(LEFT, FORWARD);
            motor_set_direction(RIGHT, FORWARD);
            motor_set_speed(LEFT, NORMAL_SPEED);
            motor_set_speed(RIGHT, LOW_SPEED);
            break;
        case VERTICAL_BACKWARD + HORIZONTAL_RIGHT:
            motor_set_direction(LEFT, REVERSE);
            motor_set_direction(RIGHT, REVERSE);
            motor_set_speed(LEFT, NORMAL_SPEED);
            motor_set_speed(RIGHT, LOW_SPEED);
            break;
        case VERTICAL_STOPPED + HORIZONTAL_STOPPED:
        default:
            motor_set_speed(LEFT, 0);
            motor_set_speed(RIGHT, 0);
            break;
    }
}
