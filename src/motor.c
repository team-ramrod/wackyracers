#include "motor.h"

void motor_init() {
    // Set the pre-scaler to 1
    TCC0_CTRLA = 0x01;
    // Set all 4 outputs and set to single slope PWM mode.
    TCC0_CTRLB = 0xF3;
    // Set the period of the output.
    // 8 kHz at 32 MHz clock = period of 4096 = 0x1000.
    TCC0_PER = 0x1000;
    // Set the output to fully on both positive and negative.
    TCC0_CCA = 0x1000;
    TCC0_CCB = 0x1000;
    TCC0_CCC = 0x1000;
    TCC0_CCD = 0x1000;
}

// Lets say speed = 0x5A
// The timer has to be set to 0x1000 * 0x5A / 0xFF
// Which (with a little rounding error) results in 0x05A5
// or:
//   0x5A << 4 + 0x5A >> 4
void motor_set_values(
        motor_t           motor,
        motor_direction_t direction,
        motor_speed_t     speed
) {
    uint8_t value = (speed << 4) + (speed >> 4);
    switch (motor + direction) {
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
    }
}
