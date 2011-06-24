/*
    Comms board UART
*/
#ifndef UART_COMMS_H
#define UART_COMMS_H

#include <stdio.h>
#include <avr/io.h>

FILE stream_cam;
FILE stream_bt;
FILE stream_debug;
FILE stream_board;

/* Initialize UART */
void uart_init(void);

#endif /* UART_COMMS_H */