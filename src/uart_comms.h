/*
    Comms board UART
*/
#ifndef UART_COMMS_H
#define UART_COMMS_H

#include <stdio.h>
#include <avr/io.h>

FILE stream_cam[1];
FILE stream_bt[1];
FILE stream_debug[1];
FILE stream_board[1];

/* Initialize UART */
void uart_init(void);

#endif /* UART_COMMS_H */
