#include "motor.h"

static void motor_apply_settings(motor_t motor);

void motor_init(motor_t motor, motor_side_t side) {
    // Initialise the motor struct default values
    motor->direction = FORWARD;
    motor->speed = 0;
    motor->side = side;

    // Set the pre-scaler to 1
    TCC0_CTRLA = 0x01;
    // Set all 4 outputs and set to single slope PWM mode.
    TCC0_CTRLB = 0xF3;
    // Set the period of the output.
    // 8 kHz at 32 MHz clock = period of 4096 = 0x1000.
    TCC0_PER = 0x1000;
    // Set the output to fully on both positive and negative.
    switch (side) {
        case LEFT:
            TCC0_CCA = 0x1000;
            TCC0_CCB = 0x1000;
            break;
        case RIGHT:
            TCC0_CCC= 0x1000;
            TCC0_CCD= 0x1000;
            break;
    }
}

void motor_set_speed(motor_t motor, motor_speed_t speed) {
    motor->speed = speed;
    motor_apply_settings(motor);
}

void motor_set_direction(motor_t motor, motor_direction_t direction) {
    motor->direction = direction;
    motor_apply_settings(motor);
}

// Lets say speed = 0x5A
// The timer has to be set to 0x1000 * 0x5A / 0xFF
// Which (with a little rounding error) results in 0x05A5
// or:
//   0x5A << 4 + 0x5A >> 4
static void motor_apply_settings(motor_t motor) {
    switch (motor->direction + motor->side) {
        case FORWARD + LEFT:
            TCC0_CCA = 0x1000;
            TCC0_CCB = (motor->speed << 4) + (motor->speed >> 4);
            break;
        case FORWARD + RIGHT:
            TCC0_CCC = 0x1000;
            TCC0_CCD = (motor->speed << 4) + (motor->speed >> 4);
            break;
        case REVERSE + LEFT:
            TCC0_CCA = (motor->speed << 4) + (motor->speed >> 4);
            TCC0_CCB = 0x1000;
            break;
        case REVERSE + RIGHT:
            TCC0_CCC = (motor->speed << 4) + (motor->speed >> 4);
            TCC0_CCD = 0x1000;
            break;
    }
}
