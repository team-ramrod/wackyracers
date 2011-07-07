#include "uart_comms.h"

int uart_putchar(char, FILE*);
int uart_getchar(FILE*);

FILE stream_cam[1]   = { FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW) };
FILE stream_bt[1]    = { FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW) };
FILE stream_debug[1] = { FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW) };
FILE stream_board[1] = { FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW) };

/*//#define get_stream() \
    if (stream == stream_debug) {      \
        uart = USARTC1;                 \
    }                                   \
    else if (stream == stream_cam) {   \
        uart = USARTD1;                 \
    }                                   \
    else if (stream == stream_bt) {    \
        uart = USARTC0;                 \
    }                                   \
    else if (stream == stream_board) { \
        uart = USARTE0;                 \
    }*/

int uart_putchar(char c, FILE * stream)
{
    volatile USART_t * uart = NULL;
    
    if (stream == stream_debug) {
        uart = &USARTC1;
    }
    else if (stream == stream_cam) {
        uart = &USARTD1;
    }
    else if (stream == stream_bt) {
        uart = &USARTC0;
    }
    else if (stream == stream_board) {
        uart = &USARTE0;
    }
    
    if (c == '\n')
        uart_putchar('\r', stream);
    
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
    else if (stream == stream_cam) {
        uart = &USARTD1;
    }
    else if (stream == stream_bt) {
        uart = &USARTC0;
    }
    else if (stream == stream_board) {
        uart = &USARTE0;
    }

    unsigned char ret;
    
    while (!(uart->STATUS & USART_RXCIF_bm));
    
    ret = uart->DATA;
    
    return ret;
}

// Init USART for camera board.
// Want 9600 baud. Have a 32 MHz clock. BSCALE = 0
// BSEL = ( 32000000 / (2^0 * 16*9600)) -1 = 207
// Fbaud = 32000000 / (2^0 * 16 * (207+1))  = 9615 bits/sec
void uart_init(void)
{
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
    
    //set up E for interboard communication

    // Set the TxD pin high - set PORTC DIR register bit 3 to 1
    PORTE.OUTSET = PIN3_bm;

    // Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
    PORTE.DIRSET = PIN3_bm;
    /* Set the RxD pin as an input*/
    PORTE.DIRCLR = PIN2_bm;
    /* Enable pulldown on recieve pin. */
    PORTE.PIN2CTRL = 0x10;

    // Set baud rate & frame format
    USARTE0.BAUDCTRLB = 0x00;
    USARTE0.BAUDCTRLA = 0xCF;

    // Set mode of operation
    USARTE0.CTRLA = 0x10;                       // enable low level interrupts
    USARTE0.CTRLC = 0x03;                       // async, no parity, 8 bit data, 1 stop bit

    // Enable transmitter and receiver
    USARTE0.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);


    //set up C for bluetooth communication

    // Set the TxD pin high - set PORTC DIR register bit 3 to 1
    PORTC.OUTSET = PIN3_bm;

    // Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
    PORTC.DIRSET = PIN3_bm;
    /* Set the RxD pin as an input*/
    PORTC.DIRCLR = PIN2_bm;
    /* Enable pulldown on recieve pin. */
    PORTC.PIN2CTRL = 0x10;

    // Set baud rate & frame format
    USARTC0.BAUDCTRLB = 0x00;
    USARTC0.BAUDCTRLA = 0xCF;

    // Set mode of operation
    USARTC0.CTRLA = 0x10;                       // enable low level interrupts
    USARTC0.CTRLC = 0x03;                       // async, no parity, 8 bit data, 1 stop bit

    // Enable transmitter and receiver
    USARTC0.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);


    //set up D for camera communication

    // Set the TxD pin high - set PORTC DIR register bit 3 to 1
    PORTD.OUTSET = PIN7_bm;

    // Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
    PORTD.DIRSET = PIN7_bm;
    /* Set the RxD pin as an input*/
    PORTD.DIRCLR = PIN6_bm;
    /* Enable pulldown on recieve pin. */
    PORTD.PIN6CTRL = 0x10;

    // Set baud rate & frame format
    USARTD1.BAUDCTRLB = 0x00;
    //USARTD1.BAUDCTRLA = 0xCF;
    USARTD1.BAUDCTRLA = 0x33;

    // Set mode of operation
    USARTD1.CTRLA = 0x10;                       // enable low level interrupts
    USARTD1.CTRLC = 0x03;                       // async, no parity, 8 bit data, 1 stop bit

    // Enable transmitter and receiver
    USARTD1.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);
}
