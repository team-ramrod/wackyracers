/*
    Comms board UART
*/
#ifndef UART_COMMS_H
#define UART_COMMS_H

#include "uart_common.h"

FILE stream_cam[1];
FILE stream_bt[1];
FILE stream_board[1];

/* Initialize UART */
void uart_init(void);

#endif /* UART_COMMS_H */
