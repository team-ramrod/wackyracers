/**
 * @file   motor_test.c
 * @author Wim Looman
 * @date   20110608
 */

#include "uart.h"
#include "clock.h"
#include "led.h"
#include "motor_controller.h"

ISR(BADISR_vect) {
    led_display_left(0x00);
    led_display_right(0x00);
}

int main(int argc, char *argv[]) {
    clock_init();
    led_init();
    motor_controller_init();
    uart_init_motor_board();

    uint8_t cmd = 0;

    while (1) {
        cmd = fgetc(&stdio_to_cam_board);
        fprintf(&stdio_to_cam_board, "%c", cmd);
        switch (cmd) {
            case 's':
            case '5':
                motor_set_movement(VERTICAL_STOPPED, HORIZONTAL_STOPPED);
                led_display(0);
                break;
            case 'w':
            case '8':
                motor_set_movement(VERTICAL_FORWARD, HORIZONTAL_STOPPED);
                led_display(1);
                break;
            case 'x':
            case '2':
                motor_set_movement(VERTICAL_BACKWARD, HORIZONTAL_STOPPED);
                led_display(2);
                break;
            case 'a':
            case '4':
                motor_set_movement(VERTICAL_STOPPED, HORIZONTAL_LEFT);
                led_display(3);
                break;
            case 'd':
            case '6':
                motor_set_movement(VERTICAL_STOPPED, HORIZONTAL_RIGHT);
                led_display(4);
                break;
            default:
                led_display(cmd-60);
                break;
        }
    }

    return 0;
}

ISR(INTERRUPT_CAM)
{
    // Do nothing
}
