/** @file   usart.c
 *  @author Simon Richards ( Sucks )
 *  @date   Created: 19 April 2011
 *
 *
 *  Status: Untested
 */
#include "common.h"
#include "uart.h"
#define BUFFER_LENGTH 10

/**
 * This structure
 */
struct uart_struct{
    void (*data_in_callback)(char *, int);
    char buffer[BUFFER_LENGTH];
};


/**
 * Anonymous data structure; this is only known to the implementation
 *  to keep the fields of the structure private.  
 */

static uart_t uart_structs[NUM_UARTS];

static USART_t *uart_devices[] = {
    &USARTC0,
    &USARTC1,
    &USARTD0,
    &USARTD1,
    &USARTE0
};

/*  uart_id_t channel;
    uint16_t baud;
    uint8_t bits;
    uart_parity_t parity;  */
/** 
 * Initialise UART for desired channel, baud rate, etc.  
 */
uart_t uart_init (uart_cfg_t *cfg) {
    uart_id_t id = cfg->channel;
    uart_devices[id]->DATA = 0x00;           // Clear the data register
    uart_devices[id]->CTRLA = 0x2A;          // Set all interrupts to MED priority
    uart_devices[id]->CTRLB = USART_RXEN_bm | USART_TXEN_bm;  // Enable sending and receiving on this channel 
    uart_devices[id]->CTRLC = (cfg->parity)<<4 | (cfg->bits-5); // User defined parity, asynchronous, 1 stop bit, 
    uart_devices[id]->BAUDCTRLA = 0xD0;
    uart_devices[id]->BAUDCTRLB = 0x00;
    return uart_structs[id];
}

/**
 * Set the callback function for incoming data
 * @param callback The callback function for incoming data
 */
void uart_set_callback(uart_t uart, void (*callback)(char *, int)) {
    uart->data_in_callback = callback;
}


/**
 * Write character.  This blocks until character written to transmit buffer.  
 */
void uart_putc (uart_t uart, char ch) {
    USARTC1.DATA = ch;
}

/**
 * Write string.  This blocks until last character written to transmit buffer.  
 */
void uart_puts (uart_t uart, char *string) {
}
