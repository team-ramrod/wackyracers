#include "uart_motor.h"
#include <stdbool.h>

int uart_putchar(char, FILE*);
int uart_getchar(FILE*);

FILE stream_board[1] = { FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW) };
FILE stream_debug[1] = { FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW) };

int uart_putchar(char c, FILE * stream)
{
    volatile USART_t * uart = NULL;
    
    if (stream == stream_debug) {
        uart = &USARTC1;
    }
    else if (stream == stream_board) {
        uart = &USARTD0;
    }
    
    /* Wait for the transmit buffer to be empty */
    while (!(uart->STATUS & USART_DREIF_bm));
    
    /* Put our character into the transmit buffer. */
    uart->DATA = c;

    return 0;
}

int uart_getchar(FILE * stream)
{
    volatile USART_t * uart = NULL;
    
    if (stream == stream_debug) {
        uart = &USARTC1;
    }
    else if (stream == stream_board) {
        uart = &USARTD0;
    }

    unsigned char ret;
    
    while (!(uart->STATUS & USART_RXCIF_bm));
    
    ret = uart->DATA;
    
    return ret;
}

void uart_init(void)
{
    static bool __initialized = false;

    if (__initialized) {
        return;
    }
    /* Configure USARTC1 for debugging. */
    PORTC.OUTSET = PIN7_bm;
    PORTC.DIRSET = PIN7_bm;
    PORTC.DIRCLR = PIN6_bm;
    /* Enable pulldown on recieve pin. */
    PORTC.PIN6CTRL = 0x10;
    
    /* Set baud rate & frame format. */
    USARTC1.BAUDCTRLB = 0x00;
    USARTC1.BAUDCTRLA = 0xCF;

    /* Set mode of operation. */
    USARTC1.CTRLA = 0x10;       // enable low level interrupts
    USARTC1.CTRLC = 0x03;       // async, no parity, 8 bit data, 1 stop bit

    /* Enable transmitter and receiver. */
    USARTC1.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);

    //set up D for camera board communication

    // Set the TxD pin high - set PORTC DIR register bit 3 to 1
    PORTD.OUTSET = PIN3_bm;

    // Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
    PORTD.DIRSET = PIN3_bm;
    /* Set the RxD pin as an input*/
    PORTD.DIRCLR = PIN2_bm;
    /* Enable pulldown on recieve pin. */
    PORTD.PIN2CTRL = 0x10;

    // Set baud rate & frame format
    USARTD0.BAUDCTRLB = 0x00;
    USARTD0.BAUDCTRLA = 0xCF;

    // Set mode of operation
    USARTD0.CTRLA = 0x10;                       // enable low level interrupts
    USARTD0.CTRLC = 0x03;                       // async, no parity, 8 bit data, 1 stop bit

    // Enable transmitter and receiver
    USARTD0.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);

    __initialized = true;
}
