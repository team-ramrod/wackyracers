/** @file   uart.h
    @author S. C. Richards, UC
    @date   27 May 2011
    @brief  Simple UART device driver interface. 

    This Header is an adaptation of MPH's original found on the ECE wiki.
*/

#include "common.h"
#include <stdint.h>
#define NUM_UARTS 5

typedef enum {UART_PARITY_NONE, UART_PARITY_EVEN, UART_PARITY_ODD} uart_parity_t;

/**
 * Configuration structure.  
 */
typedef struct uart_cfg_struct {
    uint8_t channel;
    uint16_t baud;
    uint8_t bits;
    uart_parity_t parity;
} uart_cfg_t;

/**
 * Used to index the uart_t structs
 */
typedef enum {
    C0,
    C1,
    D0,
    D1,
    E0
} uart_id_t;

/**
 * Anonymous data structure; this is only known to the implementation
 *  to keep the fields of the structure private.  
 */
typedef struct uart_struct *uart_t;

/*
 * Initialise UART for desired channel, baud rate, etc.  
 */
uart_t uart_init (uart_cfg_t *uart_cfg);

/**
 * Set the callback function for incoming data
 * @param callback The callback function for incoming data
 */
void uart_set_callback( void (*callback)(char *, int));


/**
 * Write character.  This blocks until character written to transmit buffer.  
 */
void uart_putc (uart_t uart, char ch);

/**
 * Write string.  This blocks until last character written to transmit buffer.  
 */
void uart_puts (uart_t uart, char *string);

