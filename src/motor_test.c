/**
 * @file   motor_test.c
 * @author Wim Looman
 * @date   20110608
 */

#include "commander.h"
#include "clock.h"
#include "led.h"
#include "button.h"
#include "motor_controller.h"
#include <avr/interrupt.h>

ISR(BADISR_vect) {
    led_display_left(0x00);
    led_display_right(0x00);
}

int main(int argc, char *argv[]) {
    clock_init();
    led_init();
    button_init();
    motor_controller_init();
    commander_init();
    interrupt_init();

    button_wait_power();

    cmd_t cmd = 0;

    while (1) {
        cmd = get_cmd();
        //cmd = fgetc(&stream_board);
        //fprintf(&stream_board, "%i ", cmd);
        switch (cmd) {
            case CMD_STOP:
                motor_set_movement(VERT_STOPPED, HORIZ_STOPPED);
                led_display(0);
                break;
            case CMD_FORWARD:
                motor_set_movement(VERT_FORWARD, HORIZ_STOPPED);
                led_display(1);
                break;
            case CMD_BACK:
                motor_set_movement(VERT_BACKWARD, HORIZ_STOPPED);
                led_display(2);
                break;
            case CMD_LEFT:
                motor_set_movement(VERT_STOPPED, HORIZ_LEFT);
                led_display(3);
                break;
            case CMD_RIGHT:
                motor_set_movement(VERT_STOPPED, HORIZ_RIGHT);
                led_display(4);
                break;
            case CMD_NONE:
                // Ignore it
                break;
            default:
                led_display(cmd);
                break;
        }
    }

    return 0;
}
