#include "motor.h"
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#define LEFT_FORWARD_PWM  MOTOR_TC.CCD
#define LEFT_REVERSE_PWM  MOTOR_TC.CCC
#define RIGHT_FORWARD_PWM MOTOR_TC.CCA
#define RIGHT_REVERSE_PWM MOTOR_TC.CCB

#define LEFT_FORWARD_EN  _BV(6)
#define LEFT_REVERSE_EN  _BV(7)
#define RIGHT_FORWARD_EN _BV(5)
#define RIGHT_REVERSE_EN _BV(4)

static struct {
    motor_direction_t direction;
    motor_speed_t speed;
} __motors[2];

static inline void __disable_path(motor_t motor, motor_direction_t direction) {
    switch (motor + direction) {
        case LEFT+FORWARD:  MOTOR_ENABLE_PORT.OUTCLR = LEFT_FORWARD_EN;  break;
        case LEFT+REVERSE:  MOTOR_ENABLE_PORT.OUTCLR = LEFT_REVERSE_EN;  break;
        case RIGHT+FORWARD: MOTOR_ENABLE_PORT.OUTCLR = RIGHT_FORWARD_EN; break;
        case RIGHT+REVERSE: MOTOR_ENABLE_PORT.OUTCLR = RIGHT_REVERSE_EN; break;
    }
}

static inline void __enable_path(motor_t motor, motor_direction_t direction) {
    switch (motor + direction) {
        case LEFT+FORWARD:  MOTOR_ENABLE_PORT.OUTSET = LEFT_FORWARD_EN;  break;
        case LEFT+REVERSE:  MOTOR_ENABLE_PORT.OUTSET = LEFT_REVERSE_EN;  break;
        case RIGHT+FORWARD: MOTOR_ENABLE_PORT.OUTSET = RIGHT_FORWARD_EN; break;
        case RIGHT+REVERSE: MOTOR_ENABLE_PORT.OUTSET = RIGHT_REVERSE_EN; break;
    }
}

static void __set_speed(
    motor_t motor,
    motor_direction_t direction,
    motor_speed_t speed
) {
    // Lets say speed = 0x5A
    // The timer has to be set to 0x1000 * 0x5A / 0xFF
    // Which (with a little rounding error) results in 0x05A5
    // or:
    //   0x5A << 4 + 0x5A >> 4
    uint16_t value = (speed << 4) + (speed >> 4);

    switch (motor + direction) {
        case LEFT+FORWARD:  LEFT_FORWARD_PWM  = value; break;
        case LEFT+REVERSE:  LEFT_REVERSE_PWM  = value; break;
        case RIGHT+FORWARD: RIGHT_FORWARD_PWM = value; break;
        case RIGHT+REVERSE: RIGHT_REVERSE_PWM = value; break;
    }
}

void motor_init() {
    // Set the pre-scaler to 1
    MOTOR_TC.CTRLA = 0x01;

    // Set the pins required for PWM to be outputs.
    PORTE.DIRSET = 0x0F;

    // Set all 4 pwm outputs to override the pins
    // and set to single slope PWM mode.
    MOTOR_TC.CTRLB = 0xF0 | 0x03;

    // Set the period of the output.
    // 8 kHz at 32 MHz clock = period of 4096 - 1 = 0x0999.
    MOTOR_TC.PER = 0x0999;

    // Set the pwm outputs to fully off.
    __set_speed(LEFT,  FORWARD, 0x0);
    __set_speed(LEFT,  REVERSE, 0x0);
    __set_speed(RIGHT, FORWARD, 0x0);
    __set_speed(RIGHT, REVERSE, 0x0);

    // Set the enable pins to outputs.
    MOTOR_ENABLE_PORT.DIRSET =   LEFT_FORWARD_EN 
                              |  LEFT_REVERSE_EN
                              | RIGHT_FORWARD_EN
                              | RIGHT_REVERSE_EN;

    // Set the reverse enables off.
    __disable_path(LEFT,  REVERSE);
    __disable_path(RIGHT, REVERSE);

    //added pause in here to prevent shorting if reverses were on
    _delay_us(0.72);

    // Set the forward enables on.
    __enable_path(LEFT,  FORWARD);
    __enable_path(RIGHT, FORWARD);

    // Set the default values for the structs.
    __motors[LEFT].direction = FORWARD;
    __motors[LEFT].speed = 0x0;

    __motors[RIGHT].direction = FORWARD;
    __motors[RIGHT].speed = 0x0;
}

void motor_set_speed(motor_t motor, motor_speed_t speed) {
    __set_speed(motor, __motors[motor].direction, speed);
}

bool motor_set_direction(motor_t motor, motor_direction_t direction) {
    static bool __currently_executing = false;
    bool temp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        temp = __currently_executing;
        __currently_executing = true;
    }

    if (temp) {
        return false;
    }

    // Turn off the pwm first.
    __set_speed(motor, __motors[motor].direction, 0x0);

    // If set to the same direction this is a no-op.
    if (__motors[motor].direction == direction) { 
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            __currently_executing = false;
        }
        return true;
    }

    // Disable the old enable pin.
    __disable_path(motor, __motors[motor].direction);

    // Pause to let mosfet turn off.  From data sheet max off time is 36 ns,
    // giving it a safety factor of 20x this comes to 0.72 us of dead time.
    // After some quick testing this actually results in a delay of ~26.1 us,
    // or a safety factor of over 700x.
    _delay_us(0.72);

    // Enable the new enable pin.
    __enable_path(motor, direction);

    // Update the motor struct.
    __motors[motor].direction = direction;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        __currently_executing = false;
    }

    return true;
}
