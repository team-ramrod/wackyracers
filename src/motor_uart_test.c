/* motor_uart_test.c
 *
 * This is used to test the UART interface on the motor board.
 *
 * It makes use of the interupt provided by uart
 *
 */
#include "charger.h"
#include "common.h"
#include "ir.h"
#include "led.h"
#include "motor_controller.h"
#include "uart.h"

#include <avr/interrupt.h>

ISR(BADISR_vect) {
    led_display_left(0);
    led_display_right(0);
}


int main(int argc, char *argv[]) {
    clock_init();
    charger_init();
    uart_init_motor_board();
    motor_controller_init();
    led_init();

    int i = 0;

    while(1) {
        if (i < 12 ) {
            led_display(i);
            fprintf(&stdio_to_cam_board, "%d\n", i);
            i++;
        }
    }
    return 0;
}

ISR(INTERRUPT_CAM)
{
    uint8_t input = fgetc(&stdio_to_cam_board);
    fprintf(&stdio_to_cam_board, "%i", input);
    led_display(input);
}
