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
#include "motor_controller.h"
#include "clock.h"

#include "debug.h"

#include <avr/interrupt.h>

ISR(BADISR_vect) {
    ERROR("main-motor", "Bad vector encountere%c", 'd'); // Stupid macro expansion requires a third argument.
    led_display_left(0);
    led_display_right(0);
}

int main(int argc, char *argv[]) {
    clock_init();

    motor_controller_init();
    led_init();
    commander_init();

    interrupt_init();

    motor_horiz_t motor_horiz = HORIZ_STOPPED;
    motor_vert_t motor_vert = VERT_STOPPED;

    while(1) {
        cmd_t cmd = get_cmd();

        // take latest command and change state
        switch (cmd) {
            case CMD_FORWARD:
                if (motor_vert == VERT_BACKWARD) {
                    motor_vert = VERT_STOPPED;
                } else {
                    motor_vert = VERT_FORWARD;
                }
                led_display_left(0);
                break;
            case CMD_BACK:
                if (motor_vert == VERT_FORWARD) {
                    motor_vert = VERT_STOPPED;
                } else {
                    motor_vert = VERT_BACKWARD;
                }
                led_display_left(2);
                break;
            case CMD_LEFT:
                if (motor_horiz == HORIZ_RIGHT) {
                    motor_horiz = HORIZ_STOPPED;
                } else {
                    motor_horiz = HORIZ_LEFT;
                }
                led_display_right(0);
                break;
            case CMD_RIGHT:
                if (motor_horiz == HORIZ_LEFT) {
                    motor_horiz = HORIZ_STOPPED;
                } else {
                    motor_horiz = HORIZ_RIGHT;
                }
                led_display_right(2);
                break;
            case CMD_STOP:
                motor_horiz = HORIZ_STOPPED;
                motor_vert = VERT_STOPPED;
                led_display_left(1);
                led_display_right(1);
                break;
            case CMD_NONE:
            default:
                //do nothing as the state does not need changing
                break;
        }

        // Now actually set the motors to the new state
        motor_set_movement(motor_vert, motor_horiz);
    }
    return 0;
}
