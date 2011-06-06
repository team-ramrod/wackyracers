/** @file   bluetooth.c
 *  @author Simon Richards
 *  @date   Created: 19 April 2011
 *
 *	Bluetooth driver
 *
 *  Status: Not tested
 */

#include "common.h"

#include "uart.h"

#include <stdint.h>
#include <stdlib.h>

uart_t bluetooth; //uart to bluetooth
uart_t motor; //uart to motor

void blue_init();

/**
 * This gets an input character followed by a \r\n echos it back to the computer
 * and sends the command to the motor board
 * Based on the UART command line process by Micheal Hayes on ECE wiki
 *  
 */
inline void blue_read_bluetooth(char *string, int length) {
    /* Echo input  */
    uart_puts(bluetooth, string);

    /*check a single character was sent*/
    if (length != 3) {
        uart_puts(bluetooth, "invalid command\r\n");
        return;
    }

    //send appropriate command based on input
    switch (string[0]) {
        case 'w':
            uart_putc(motor, CMD_FORWARD);
            break;

        case 'a':
            uart_putc(motor, CMD_LEFT);
            break;

        case 's':
            uart_putc(motor, CMD_STOP);
            break;

        case 'd':
            uart_putc(motor, CMD_RIGHT);
            break;

        case 'x':
            uart_putc(motor, CMD_BACK);
            break;

        case 'c':
            uart_putc(motor, CMD_GET_IMAGE);
            break;

        case 'z':
            uart_putc(motor, CMD_TOGGLE);
            break;

        default:
            uart_puts(bluetooth, "invalid command\n");
            break;
    }
}

