/**
 * @file   motor_test.c
 * @author Wim Looman
 * @date   20110608
 */

#include "clock.h"
#include "led.h"
#include "motor.h"

#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char *argv[]) {
    uint8_t left = 0, right = 0;

    clock_init();
    led_init();
    motor_init();

    while (1) {
        led_display_left(left);
        led_display_right(right);

        motor_set_speed(LEFT,  left * 28);
        motor_set_speed(RIGHT, right * 28);

        left  = rand() % 10;
        right = rand() % 10;

        _delay_ms(4000.0);
    }

    return 0;
}
