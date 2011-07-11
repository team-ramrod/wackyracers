/** @file   main-motor.c
 *  @author Simon Richards, Henry Jenkins
 *  @date   Created: 19 April 2011
 *
 *	Initiation calls and main loop.
 *
 *
 *  Status: Untested
*/
#include "common.h"
#include "commander.h"
#include "led.h"
#include "chassis.h"
#include "clock.h"
#include "uart_motor.h"

#include "debug.h"

#include <avr/interrupt.h>

ISR(BADISR_vect) {
    ERROR("main-motor", "Bad vector encountered, PMIC.STATUS = [%x]", PMIC.STATUS);
    led_display_left(0);
    led_display_right(0);
}

bool increment_speed(motor_speed_t *speed) {
    if (*speed < 60) {
        *speed = *speed + 20;
        return true;
    } else {
        return false;
    }
}

bool decrement_speed(motor_speed_t *speed) {
    if (*speed > -60) {
        *speed = *speed - 20;
        return true;
    } else {
        return false;
    }
}

int main(int argc, char *argv[]) {
    clock_init();

    uart_init();

    chassis_init();
    led_init();
    commander_init();

    interrupt_init();

    motor_speed_t speed = 0;
    chassis_direction_t direction = CENTER;

    while(1) {
        cmd_t cmd = get_cmd();

        // take latest command and change state
        switch (cmd) {
            case CMD_FORWARD:
                if (increment_speed(&speed)) {
                    chassis_set_speed(speed);
                }
                break;
            case CMD_BACK:
                if (decrement_speed(&speed)) {
                    chassis_set_speed(speed);
                }
                break;
            case CMD_LEFT:
                if (direction == RIGHT) {
                    direction = CENTER;
                } else {
                    direction = LEFT;
                }
                chassis_set_direction(direction);
                break;
            case CMD_RIGHT:
                if (direction == LEFT) {
                    direction = CENTER;
                } else {
                    direction = RIGHT;
                }
                chassis_set_direction(direction);
                break;
            case CMD_STOP:
                direction = CENTER;
                speed = 0;
                chassis_set_speed(speed);
                chassis_set_direction(direction);
                break;
            case CMD_NONE:
            default:
                //do nothing as the state does not need changing
                break;
        }
    }
    return 0;
}
