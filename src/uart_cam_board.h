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

#define INTERRUPT_BLUE USARTC0_RXC_vect
#define INTERRUPT_MOTOR USARTE0_RXC_vect
#define INTERRUPT_CAM USARTD0_RXC_vect

//initilizes uart on camera board
void uart_init_cam_board (void);

//sends data to bluetooth
int uart_putchar_blue(char c, FILE *stream);

//sends data to motor board
int uart_putchar_motor(char c, FILE *stream);

//sends data to camera
int uart_putchar_cam(char c, FILE *stream);


//gets data from bluetooth
int uart_getchar_blue(FILE *stream);

//gets data from motor
int uart_getchar_motor(FILE *stream);

//gets data from camera
int uart_getchar_cam(FILE *stream);

#endif	/* UART_CAM_BOARD_H */

