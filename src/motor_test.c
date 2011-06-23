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
        fprintf(&stdio_to_motor,"asdf");
    }

    return 0;
}
