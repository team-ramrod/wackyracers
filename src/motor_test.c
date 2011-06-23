/**
 * @file   motor_test.c
 * @author Wim Looman
 * @date   20110608
 */

#include "uart.h"
#include "clock.h"
#include "led.h"
#include "motor_controller.h"

int main(int argc, char *argv[]) {
    clock_init();
    led_init();
    motor_controller_init();
    uart_init_motor_board();

    uint8_t cmd = 0;

    while (1) {
        cmd = uart_getchar_debug(&stdio_to_cam_board);
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
            default:
                led_display(cmd);
                break;
        }
    }

    return 0;
}
