#ifndef UART_COMMON_H
#define UART_COMMON_H

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define INTERRUPT_DEBUG USARTC1_RXC_vect

FILE stream_debug[1];

#endif /* UART_COMMON_H */
