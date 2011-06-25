/*
    Motor board UART.
*/
#ifndef UART_MOTOR_H
#define UART_MOTOR_H

#include "uart_common.h"

//Use this for interupts from the bluetooth module
#define INTERRUPT_BLUE USARTC0_RXC_vect
//Use this for when a signal is coming from motor board to cam board's UART
#define INTERRUPT_MOTOR USARTE0_RXC_vect
// ummm... fuck... signl from the image sensor OR from cam_board to motor board
#define INTERRUPT_CAM USARTD0_RXC_vect

FILE stream_board[1];

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

ISR(INTERRUPT_CAM) {
    // Hacky testing code for using keyboard to control it:
    uint8_t value = getc(stream_board);
    cmd_t ret = CMD_GET_IMAGE;
    switch (value) {
        case 'w': ret = CMD_FORWARD; break;
        case 's': ret = CMD_STOP; break;
        case '`': ret = CMD_ASSUME_CTRL; break;
    }
    if (__motor_board_callback != NULL) {
        __motor_board_callback(ret);
    }

    // Actual code:
    /*
    if (__motor_board_callback != NULL) {
        __motor_board_callback(getc(stream_board));
    }
    */
}
#endif

#endif /* UART_MOTOR_H */
