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
    uint8_t num = 0;

    clock_init();
    led_init();
    motor_init();


    while (1) {
        led_display(num = (num + 1) % 100);
        //motor_set_speed(LEFT, ((num - 1) % 2) * 20);
        motor_set_speed(RIGHT, num * 5);
        //motor_set_speed(LEFT,  left * 28);
        //motor_set_speed(RIGHT, right * 28);

        _delay_ms(12000.0);
    }

    return 0;
}
