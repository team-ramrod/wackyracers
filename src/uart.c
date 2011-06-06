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
    char data_in;
    char data_out;
    void (*data_in_callback)(char *, int);
    char buffer[BUFFER_LENGTH];
};

/**
 * Anonymous data structure; this is only known to the implementation
 *  to keep the fields of the structure private.  
 */

static uart_struct uart_structs[NUM_UARTS];

static USART_t uart_devices[NUM_UARTS] = {
    USARTC0,
    USARTC1,
    USARTD0,
    USARTD1,
    USARTE0
}

/*  uart_id_t channel;
    uint16_t baud;
    uint8_t bits;
    uart_parity_t parity;  */
/** 
 * Initialise UART for desired channel, baud rate, etc.  
 */
uart_t uart_init (uart_cfg_t *uart_cfg) {
    uart_devices[uart_cfg_t.channel].DATA = 0x00;
    uart_devices[uart_cfg_t.channel].CTRLA = 0x2A;
    uart_devices[uart_cfg_t.channel].CTRLB = 0x18;
    uart_devices[uart_cfg_t.channel].CTRLC = 0x03;
    uart_devices[uart_cfg_t.channel].BAUDCTRLA = 0xD0;
    uart_devices[uart_cfg_t.channel].BAUDCTRLB = 0x00;
    return uart_structs + uart_cfg_t.channel;
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
void uart_putc (uart_t *uart, char ch) {

}

/**
 * Write string.  This blocks until last character written to transmit buffer.  
 */
void uart_puts (uart_t *uart, char *string) {
    //set_tx1_mode;
    while (usartc1_busy);
        USARTC1.DATA = byte;
}
