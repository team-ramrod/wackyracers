/*
    Motor board UART.
*/
#ifndef UART_MOTOR_H
#define UART_MOTOR_H

#include "uart_common.h"

// Interrupt for data from the other board.
#define INTERRUPT_BOARD USARTD0_RXC_vect

/* Initialize UART */
void uart_init(void);

/**
 * The callback for the motor board. Define ENABLE_UART_MOTOR_CALLBACK if you 
 * wish to have this callback available.  Can only be defined in one
 * translation unit or bad things will occur.
 */
#ifdef ENABLE_UART_MOTOR_CALLBACK

#include "common.h"

typedef void (*uart_cmd_callback) (cmd_t);
uart_cmd_callback __motor_board_callback = NULL;

/**
 * Sets the callback to be run when a command is recieved from the camera board
 * by the motor board.  Handles reading the input and decoding the requested
 * command.
 *
 * @parm callback The callback to be run, takes in the command as a paramenter.
 */
static void uart_set_motor_board_callback(uart_cmd_callback callback) {
    __motor_board_callback = callback;
}

ISR(INTERRUPT_BOARD) {
    if (__motor_board_callback != NULL) {
        __motor_board_callback(getc(stream_board));
    }
}
#endif

#endif /* UART_MOTOR_H */
