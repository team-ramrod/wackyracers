#include "motor.h"
#include <avr/io.h>
#include <util/delay.h>

#define LEFT_FORWARD_PWM  TCC0_CCC
#define LEFT_REVERSE_PWM  TCC0_CCD
#define RIGHT_FORWARD_PWM TCC0_CCA
#define RIGHT_REVERSE_PWM TCC0_CCB

#define ENABLE_PORT     PORTD_OUT
#define ENABLE_PORT_DIR PORTD_DIR

#define LEFT_FORWARD_EN  _BV(6)
#define LEFT_REVERSE_EN  _BV(7)
#define RIGHT_FORWARD_EN _BV(5)
#define RIGHT_REVERSE_EN _BV(4)

static struct {
    motor_direction_t direction;
} __motors[2];

static inline void __disable_path(motor_t motor, motor_direction_t direction) {
    switch (motor + direction) {
        case LEFT + FORWARD:  ENABLE_PORT &= ~LEFT_FORWARD_EN;  break;
        case LEFT + REVERSE:  ENABLE_PORT &= ~LEFT_REVERSE_EN;  break;
        case RIGHT + FORWARD: ENABLE_PORT &= ~RIGHT_FORWARD_EN; break;
        case RIGHT + REVERSE: ENABLE_PORT &= ~RIGHT_REVERSE_EN; break;
    }
}

static inline void __enable_path(motor_t motor, motor_direction_t direction) {
    switch (motor + direction) {
        case LEFT + FORWARD:  ENABLE_PORT |= LEFT_FORWARD_EN;  break;
        case LEFT + REVERSE:  ENABLE_PORT |= LEFT_REVERSE_EN;  break;
        case RIGHT + FORWARD: ENABLE_PORT |= RIGHT_FORWARD_EN; break;
        case RIGHT + REVERSE: ENABLE_PORT |= RIGHT_REVERSE_EN; break;
    }
}

// Lets say speed = 0x5A
// The timer has to be set to 0x1000 * 0x5A / 0xFF
// Which (with a little rounding error) results in 0x05A5
// or:
//   0x5A << 4 + 0x5A >> 4
static void __set_speed(
    motor_t motor,
    motor_direction_t direction,
    motor_speed_t speed
) {
    uint8_t value = (speed << 4) + (speed >> 4);
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

void motor_init() {
    // Set the pre-scaler to 1
    TCC0_CTRLA = 0x01;

    // Set all 4 pwm outputs and set to single slope PWM mode.
    TCC0_CTRLB = 0xF3;

    // Set the period of the output.
    // 8 kHz at 32 MHz clock = period of 4096 = 0x1000.
    TCC0_PER = 0x1000;

    // Set the pwm outputs to fully off.
    __set_speed(LEFT,  FORWARD, 0x0);
    __set_speed(LEFT,  REVERSE, 0x0);
    __set_speed(RIGHT, FORWARD, 0x0);
    __set_speed(RIGHT, REVERSE, 0x0);

    // Set the enable pins to outputs.
    ENABLE_PORT_DIR |= LEFT_FORWARD_EN 
                    |  LEFT_REVERSE_EN
                    | RIGHT_FORWARD_EN
                    | RIGHT_REVERSE_EN;

    // Set the reverse enables off.
    __disable_path(LEFT,  REVERSE);
    __disable_path(RIGHT, REVERSE);

    // Set the forward enables on.
    __enable_path(LEFT,  FORWARD);
    __enable_path(RIGHT, FORWARD);

    // Set the default values for the structs.
    __motors[LEFT].direction = FORWARD;
    __motors[RIGHT].direction = FORWARD;
}

void motor_set_speed(motor_t motor, motor_speed_t speed) {
    __set_speed(motor, __motors[motor].direction, speed);
}

void motor_set_direction(motor_t motor, motor_direction_t direction) {
    // Turn off the pwm first.
    __set_speed(motor, __motors[motor].direction, 0x0);

    // If set to the same direction this is a no-op.
    if (__motors[motor].direction == direction) { 
        return;
    }

    // Disable the old enable pin.
    __disable_path(motor, __motors[motor].direction);

    // Pause to let mosfet turn off.  From data sheet max off time is 36 ns,
    // giving it a whole us this gives it a safety factor of 27x.
    _delay_us(1.0);

    // Enable the new enable pin.
    __enable_path(motor, direction);

    // Update the struct.
    __motors[motor].direction = direction;
}
