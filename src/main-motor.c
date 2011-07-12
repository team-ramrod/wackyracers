/** @file   main-motor.c
 *  @author Simon Richards, Henry Jenkins
 *  @date   Created: 19 April 2011
 *
 *	Initiation calls and main loop.
 *
 *
 *  Status: working
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

    bool changed = true;
    
    clock_init();

    uart_init();

    chassis_init();
    led_init();
    commander_init();

    interrupt_init();

    motor_speed_t speed = 0;
    chassis_direction_t direction = CENTRE;

    while(1) {
        cmd_t cmd = get_cmd();

        // take latest command and change state
        switch (cmd) {
            case CMD_FORWARD:
                if (increment_speed(&speed)) {
                    changed = true;
                }
                break;
            case CMD_BACK:
                if (decrement_speed(&speed) ) {
                    changed = true;
                }
                break;
            case CMD_LEFT:
                if (direction == RIGHT) {
                    direction = CENTRE;
                } else {
                    direction = LEFT;
                }
                changed = true;
                break;
            case CMD_RIGHT:
                if (direction == LEFT) {
                    direction = CENTRE;
                } else {
                    direction = RIGHT;
                }
                changed = true;
                break;
            case CMD_STOP:
                direction = CENTRE;
                speed = 0;
                changed = true;
                break;
            case CMD_NONE:
            default:
                //do nothing as the state does not need changing
                break;
        }
        if (changed) {
            chassis_set_speed(speed);
            chassis_set_direction(direction);
            changed = false;
        }
    }
    return 0;
}
