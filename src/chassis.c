#include "chassis.h"

#include <util/atomic.h>
#include <util/delay.h>

#include "debug.h"

#define LEFT_FORWARD_PWM  MOTOR_TC.CCC
#define LEFT_REVERSE_PWM  MOTOR_TC.CCD
#define RIGHT_FORWARD_PWM MOTOR_TC.CCB
#define RIGHT_REVERSE_PWM MOTOR_TC.CCA

#define LEFT_FORWARD_EN  _BV(7)
#define LEFT_REVERSE_EN  _BV(6)
#define RIGHT_FORWARD_EN _BV(4)
#define RIGHT_REVERSE_EN _BV(5)
#define HIGH_SPEED       20

#define set_servo_low()     PORTR.OUTCLR = bit(0)
#define set_servo_high()    PORTR.OUTSET = bit(0)
#define SERVO_LEFT_PERIOD   (2^16) - 18129
#define SERVO_HARD_LEFT_PERIOD   (2^16) - 16129
#define SERVO_RIGHT_PERIOD  (2^16) - 28258
#define SERVO_HARD_RIGHT_PERIOD  (2^16) - 30258
#define SERVO_CENTRE_PERIOD (2^16) - 22693

static uint16_t servo_period;

static struct {
    motor_direction_t direction;
    motor_speed_t speed;
} __motor;

static inline void __disable_path(motor_direction_t direction) {
    VERBOSE(
        "motor",
        "Disabling drain [%s].",
        direction == FORWARD ? "FORWARD" : "REVERSE"
    );

    switch (direction) {
        case FORWARD:
            MOTOR_ENABLE_PORT.OUTCLR = LEFT_FORWARD_EN;
            MOTOR_ENABLE_PORT.OUTCLR = RIGHT_FORWARD_EN;
            break;
        case REVERSE:
            MOTOR_ENABLE_PORT.OUTCLR = LEFT_REVERSE_EN;
            MOTOR_ENABLE_PORT.OUTCLR = RIGHT_REVERSE_EN;
            break;
    }
}

static inline void __enable_path(motor_direction_t direction) {
    VERBOSE(
        "motor",
        "Enabling drain [%s %s].",
        direction == FORWARD ? "FORWARD" : "REVERSE"
    );

    switch (direction) {
        case FORWARD:
            MOTOR_ENABLE_PORT.OUTSET = LEFT_FORWARD_EN;
            MOTOR_ENABLE_PORT.OUTSET = RIGHT_FORWARD_EN;
            break;
        case REVERSE:
            MOTOR_ENABLE_PORT.OUTSET = LEFT_REVERSE_EN;
            MOTOR_ENABLE_PORT.OUTSET = RIGHT_REVERSE_EN;
            break;
    }
}

static void __set_speed(motor_direction_t direction, motor_speed_t speed) {
    // Lets say speed = 0x5A
    // The timer has to be set to 0x1000 * 0x5A / 0xFF
    // Which (with a little rounding error) results in 0x05A5
    // or:
    //   0x5A << 4 + 0x5A >> 4
    uint16_t value = (speed << 4) + (speed >> 4);

    VERBOSE(
        "motor",
        "Changing speed of PWM [%s] to [%i].",
        direction == FORWARD ? "FORWARD" : "REVERSE",
        speed
    );

    switch (direction) {
        case FORWARD:
            LEFT_FORWARD_PWM = value;
            RIGHT_FORWARD_PWM = value;
            break;
        case REVERSE:
            LEFT_REVERSE_PWM = value;
            RIGHT_REVERSE_PWM = value;
            break;
    }
}
void chassis_init() {
    DEBUG("motor", "Started initialization.");
    // Set the pre-scaler to 1
    MOTOR_TC.CTRLA = 0x04;

    // Set the pins required for PWM to be outputs.
    PORTE.DIRSET = 0x0F;

    // Set all 4 pwm outputs to override the pins
    // and set to single slope PWM mode.
    MOTOR_TC.CTRLB = 0xF0 | 0x03;

    // Set the period of the output.
    // 8 kHz at 32 MHz clock = period of 4096 - 1 = 0x0999.
    MOTOR_TC.PER = 0x0999;

    // Set the pwm outputs to fully off.
    __set_speed(FORWARD, 0x0);
    __set_speed(REVERSE, 0x0);

    // Set the enable pins to outputs.
    MOTOR_ENABLE_PORT.DIRSET =   LEFT_FORWARD_EN 
                              |  LEFT_REVERSE_EN
                              | RIGHT_FORWARD_EN
                              | RIGHT_REVERSE_EN;

    // Set the reverse enables off.
    __disable_path(REVERSE);

    //added pause in here to prevent shorting if reverses were on
    _delay_us(0.72);

    // Set the forward enables on.
    __enable_path(FORWARD);

    // Set the default values for the structs.
    __motor.direction = FORWARD;
    __motor.speed = 0x0;

    // Setup the servo 
    chassis_set_direction(CENTRE);
    // Update Timer
    SERVO_UPDATE_TC.CTRLA = 0x07; // clk/1024
    SERVO_UPDATE_TC.PER   = 0x0280; // period of 640 ticks
    SERVO_UPDATE_TC.INTCTRLA = 0x03;
    
    // Strobe Timer
    SERVO_STROBE_TC.INTCTRLA = 0x03;

    PORTR.DIRSET = bit(0);


    DEBUG("motor", "Finished initialization.");

}

bool __motor_set_direction(motor_direction_t direction) {
    DEBUG(
            "motor",
            "Changing direction of motor to [%s].",
            direction == FORWARD ? "FORWARD" : "REVERSE"
         );

    static bool __currently_executing = false;
    bool temp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        temp = __currently_executing;
        __currently_executing = true;
    }

    if (temp) {
        DEBUG("motor", "Canceling direction change.");
        return false;
    }

    // Turn off the pwm first.
    __set_speed(__motor.direction, 0x0);

    // If set to the same direction this is a no-op.
    if (__motor.direction == direction) { 
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            __currently_executing = false;
        }
        return true;
    }

    // Disable the old enable pin.
    __disable_path(__motor.direction);

    // Pause to let mosfet turn off.  From data sheet max off time is 36 ns,
    // giving it a safety factor of 20x this comes to 0.72 us of dead time.
    // After some quick testing this actually results in a delay of ~26.1 us,
    // or a safety factor of over 700x.
    _delay_us(0.72);

    // Enable the new enable pin.
    __enable_path(direction);

    // Update the motor struct.
    __motor.direction = direction;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        __currently_executing = false;
    }

    return true;
}

void chassis_set_speed(motor_speed_t speed) {
    DEBUG(
            "chassis",
            "Changing speed to [%i].",
            speed
         );

    while (!__motor_set_direction(speed > 0 ? FORWARD : REVERSE)) {
        // Do nothing
    }
    __motor.speed = abs(speed);
    __set_speed(__motor.direction, abs(speed));
}

void chassis_set_direction(chassis_direction_t direction) {
    switch (direction) {
        case LEFT:
            servo_period = (__motor.speed > HIGH_SPEED) ?
                SERVO_LEFT_PERIOD : SERVO_HARD_LEFT_PERIOD;
            break;
        case RIGHT:
            servo_period = (__motor.speed > HIGH_SPEED) ?
                SERVO_RIGHT_PERIOD : SERVO_HARD_RIGHT_PERIOD;
            break;
        case CENTRE:
            servo_period = SERVO_CENTRE_PERIOD;
            break;
        default:
            break;
    }
}

ISR(SERVO_UPDATE_TC_OVF_vect, ISR_NOBLOCK) {
    SERVO_STROBE_TC.CTRLA = 0x02; // clk/2
    SERVO_STROBE_TC.CNT   = servo_period; 
    set_servo_high();
}

ISR(SERVO_STROBE_TC_OVF_vect, ISR_NOBLOCK) {
    set_servo_low();
    SERVO_STROBE_TC.CTRLA = 0x00;
}
