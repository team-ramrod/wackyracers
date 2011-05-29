#include "motor.h"
#include <avr/io.h>
#include <util/delay.h>

#define LEFT_FORWARD_PWM TCC0_CCC
#define LEFT_REVERSE_PWM TCC0_CCD
#define RIGHT_FORWARD_PWM TCC0_CCA
#define RIGHT_REVERSE_PWM TCC0_CCB

static struct {
    motor_direction_t direction;
} __motors[2];

// Lets say speed = 0x5A
// The timer has to be set to 0x1000 * 0x5A / 0xFF
// Which (with a little rounding error) results in 0x05A5
// or:
//   0x5A << 4 + 0x5A >> 4
static uint8_t __translate_speed(motor_speed_t speed) {
    return (speed << 4) + (speed >> 4);
}

static void __set_speed(motor_t motor, motor_direction_t direction, uint8_t speed) {
}

void motor_init() {
    // Set the pre-scaler to 1
    TCC0_CTRLA = 0x01;

    // Set all 4 pwm outputs and set to single slope PWM mode.
    TCC0_CTRLB = 0xF3;

    // Set the period of the output.
    // 8 kHz at 32 MHz clock = period of 4096 = 0x1000.
    TCC0_PER = 0x1000;

    // Set the pwm outputs to fully off.
    LEFT_FORWARD_PWM = 0x0000;
    LEFT_REVERSE_PWM = 0x0000;
    RIGHT_FORWARD_PWM = 0x0000;
    RIGHT_FORWARD_PWM = 0x0000;

    // Set the enable pins to outputs.

    // Set the reverse enables off.

    // Set the forward enables on.

    // Set the default values for the structs.
    __motors[LEFT].direction = FORWARD;
    __motors[RIGHT].direction = FORWARD;
}

void motor_set_speed(motor_t motor, motor_speed_t speed) {
    __set_speed(motor, __motors[motor].direction, __translate_speed(speed));
    /*switch (motor + direction) {
        case LEFT + FORWARD:
            TCC0_CCA = 0x1000;
            TCC0_CCB = value;
            break;
        case RIGHT + FORWARD:
            TCC0_CCC = 0x1000;
            TCC0_CCD = value;
            break;
        case LEFT + REVERSE:
            TCC0_CCA = value;
            TCC0_CCB = 0x1000;
            break;
        case RIGHT + REVERSE:
            TCC0_CCC = value;
            TCC0_CCD = 0x1000;
            break;
    }*/
}

void motor_set_direction(motor_t motor, motor_direction_t direction) {
    if (__motors[motor].direction == direction) { return; }

    __set_speed(motor, __motors[motor].direction, __translate_speed(0));

    __motors[motor].direction = direction;

    __set_speed(motor, __motors[motor].direction, __translate_speed(0));

    switch (motor) {
        case LEFT:
            break;
        case RIGHT:
            break;
    }
}
