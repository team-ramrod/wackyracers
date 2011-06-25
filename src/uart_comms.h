/*
    Comms board UART
*/
#ifndef UART_COMMS_H
#define UART_COMMS_H

#include "uart_common.h"

// Interrupt for data from the image sensor.
#define INTERRUPT_CAM USARTD0_RXC_vect

// Interrupt for data from the bluetooth.
#define INTERRUPT_BT USARTC0_RXC_vect

// Interrupt for data from the other board.
#define INTERRUPT_BOARD USARTE0_RXC_vect

FILE stream_cam[1];
FILE stream_bt[1];
FILE stream_board[1];

/* Initialize UART */
void uart_init(void);

#endif /* UART_COMMS_H */
