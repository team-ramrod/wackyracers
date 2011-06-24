#ifndef UART_CAM_BOARD_H
#define	UART_CAM_BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "clock.h"
#include "led.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define UART_BLUE USARTC0
#define UART_MOTOR USARTE0
#define UART_CAM USARTD0

//Use this for interupts from the bluetooth module
#define INTERRUPT_BLUE USARTC0_RXC_vect
//Use this for when a signal is coming from motor board to cam board's UART
#define INTERRUPT_MOTOR USARTE0_RXC_vect
// ummm... fuck... signl from the image sensor OR from cam_board to motor board
#define INTERRUPT_CAM USARTD0_RXC_vect

// THe stdio for communicating to the BT module
FILE stdio_blue;
// if you are the cam_board and you want to send to the motor board
FILE stdio_to_motor_board;
// if you are the motor board and want to talk to cam_board
FILE stdio_to_cam_board;
// image sensor coms
FILE stdio_cam;

/******************************************************************************
 * Documentation required to be added to this header file:                    *
 *                                                                            *
 *  * Blocking?  Do any of these functions block, or are they asynchronous.   *
 *                                                                            *
 *  * stream?    What stream does it want, I want to run uart_getchar_debug   *
 *               but I have no idea what stream to pass it.                   *
 *                                                                            *
 *  * int?       Are the values returned signed or unsigned?  Shouldn't this  *
 *               be a uint8_t.
 *                                                                            *
 ******************************************************************************/

/**
 * The callback for the motor board. Define ENABLE_UART_MOTOR_CALLBACK if you 
 * wish to have this callback available.  Can only be defined in one
 * translation unit or bad things will occur.
 */
#ifdef ENABLE_UART_MOTOR_CALLBACK
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
    if (__motor_board_callback != NULL) {
        __motor_board_callback(getc(&stdio_to_cam_board));
    } else {
        getc(&stdio_to_cam_board);
    }
}
#endif

//initilizes uart on camera board
void uart_init_cam_board (void);

//initilizes uart on camera board
void uart_init_motor_board (void);

#endif	/* UART_CAM_BOARD_H */

