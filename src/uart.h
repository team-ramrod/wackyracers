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


//initilizes uart on camera board
void uart_init_cam_board (void);

//initilizes uart on camera board
void uart_init_motor_board (void);

int uart_putchar_debug(char c, FILE *stream);

//sends data to bluetooth
int uart_putchar_blue(char c, FILE *stream);

//sends data to motor board
int uart_putchar_motor(char c, FILE *stream);

//sends data to computer
int uart_putchar_debug(char c, FILE *stream);

//sends data to camera
int uart_putchar_cam(char c, FILE *stream);

//sends data to camera board
int uart_putchar_cam_board(char c, FILE *stream);

//gets data from bluetooth
int uart_getchar_blue(FILE *stream);

//gets data from motor
int uart_getchar_motor(FILE *stream);

//gets data from computer
int uart_getchar_debug(FILE *stream);

//gets data from camera
int uart_getchar_cam(FILE *stream);

//gets data from camera
int uart_getchar_cam_board(FILE *stream);

#endif	/* UART_CAM_BOARD_H */

