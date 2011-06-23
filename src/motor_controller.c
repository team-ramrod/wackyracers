#include "motor_controller.h"
#include "motor.h"

void motor_controller_init() {
    motor_init();
}

void motor_set_movement(motor_vertical_t vert, motor_horizontal_t horiz) {
    switch(vert + horiz) {
        case VERTICAL_STOPPED + HORIZONTAL_STOPPED:
            motor_set_speed(LEFT, 0);
            motor_set_speed(RIGHT, 0);
            break;
        case VERTICAL_FORWARD + HORIZONTAL_STOPPED:
            motor_set_direction(LEFT, FORWARD);
            motor_set_direction(RIGHT, FORWARD);
            motor_set_speed(LEFT, 10);
            motor_set_speed(RIGHT, 10);
            break;
    }
}
