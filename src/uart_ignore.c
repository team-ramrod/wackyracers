#include "uart.h"

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);


/* UNI-DIRECTIONAL STREAMS ------------------------------------------------ */

/* Communication stream FROM motor board TO comms board. */
/* NOTE Currently unused. */
FILE stream_m2c = FDEV_SETUP_STREAM (uart_putchar, uart_getchar, _FDEV_SETUP_RW);
//FILE * stream_m2c = { FDEV
/* Communication stream FROM comms board TO motor board. */
FILE stream_c2m = FDEV_SETUP_STREAM (uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/* BI-DIRECTIONAL STREAMS ------------------------------------------------- */

/* Communication stream for camera. Only used within comms board. */
FILE stream_cam = FDEV_SETUP_STREAM (uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/* Debug stream. Board <-> PC communications. */
FILE stream_debug = FDEV_SETUP_STREAM (uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/* Communication stream for bluetooth. Only used within comms board. */
FILE stream_bt = FDEV_SETUP_STREAM (uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/* ------------------------------------------------------------------------ */

int uart_putchar(char c, FILE * stream)
{
    volatile USART_t uart;
    
    if (stream == &stderr) {
        uart = USARTD0;
    }
    else if (stream == &stdio_blue) {
        uart = USARTC0;
    }
    else if (stream == &stdio_to_motor_board) {
        uart = USARTE0;
    }
    else if (stream == &stdio_to_cam_board) {
        uart = USARTD0;
    }
    else if (stream == stdio_cam) {
        uart = USARTD0;
    }

    if (c == '\n')
        uart_putchar('\r', stream);

    // Wait for the transmit buffer to be empty
    while ( !( uart.STATUS & USART_DREIF_bm) );

    // Put our character into the transmit buffer
    uart.DATA = c;

    return 0;
}


//send data to camera board
int uart_putchar_cam_board (char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar_cam('\r', stream);

    // Wait for the transmit buffer to be empty
    while ( !( USARTD0.STATUS & USART_DREIF_bm) );

    // Put our character into the transmit buffer
    USARTD0.DATA = c;

    return 0;
}

int uart_getchar(FILE * stream)
{
    volatile USART_t uart;
    
    if (stream == &stderr) {
        uart = USARTD0;
    }
    else if (stream == &stdio_blue) {
        uart = USARTC0;
    }
    else if (stream == &stdio_to_motor_board) {
        uart = USARTE0;
    }
    else if (stream == &stdio_to_cam_board) {
        uart = USARTD0;
    }
    else if (stream == stdio_cam) {
        uart = USARTD0;
    }

    unsigned char ret;
    
    while (!(uart.STATUS & USART_RXCIF_bm) );
    
    ret = uart.DATA;
    
    return ret;
}

//get data from bluetooth
int uart_getchar_blue (FILE *stream)
{
    unsigned char ret;

    while ( !( USARTC0.STATUS & USART_RXCIF_bm) );

    // Put our character into the transmit buffer
    ret = USARTC0.DATA;

    return ret;
}

//get data from bluetooth
int uart_getchar_motor (FILE *stream)
{
    unsigned char ret;

    while ( !( USARTE0.STATUS & USART_RXCIF_bm) );

    // Put our character into the transmit buffer
    ret = USARTE0.DATA;

    return ret;
}

//get data from camera
int uart_getchar_cam (FILE *stream)
{
    unsigned char ret;

    while ( !( USARTD0.STATUS & USART_RXCIF_bm) );

    // Put our character into the transmit buffer
    ret = USARTD0.DATA;

    return ret;
}

//get data from camera board
int uart_getchar_cam_board (FILE *stream)
{
    unsigned char ret;

    while ( !( USARTD0.STATUS & USART_RXCIF_bm) );

    // Put our character into the transmit buffer
    ret = USARTD0.DATA;

    return ret;
}

//get data from computer
int uart_getchar_debug (FILE *stream)
{
    unsigned char ret;

    while ( !( USARTD0.STATUS & USART_RXCIF_bm) );

    // Put our character into the transmit buffer
    ret = USARTD0.DATA;

    return ret;
}


// Init USART for camera board.
// Want 9600 baud. Have a 32 MHz clock. BSCALE = 0
// BSEL = ( 32000000 / (2^0 * 16*9600)) -1 = 207
// Fbaud = 32000000 / (2^0 * 16 * (207+1))  = 9615 bits/sec
void uart_init_cam_board (void)
{
    //set up E for interboard communication

    // Set the TxD pin high - set PORTC DIR register bit 3 to 1
    PORTE.OUTSET = PIN3_bm;

    // Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
    PORTE.DIRSET = PIN3_bm;
    /* Set the RxD pin as an input*/
    PORTE.DIRCLR = PIN2_bm;

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

    // Set baud rate & frame format
    USARTD0.BAUDCTRLB = 0x00;
    USARTD0.BAUDCTRLA = 0xCF;

    // Set mode of operation
    USARTD0.CTRLA = 0x10;                       // enable low level interrupts
    USARTD0.CTRLC = 0x03;                       // async, no parity, 8 bit data, 1 stop bit

    // Enable transmitter and receiver
    USARTD0.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);
}

// Init USART for camera board.
// Want 9600 baud. Have a 32 MHz clock. BSCALE = 0
// BSEL = ( 32000000 / (2^0 * 16*9600)) -1 = 207
// Fbaud = 32000000 / (2^0 * 16 * (207+1))  = 9615 bits/sec
void uart_init_motor_board (void)
{
    //set up D for camera board communication

    // Set the TxD pin high - set PORTC DIR register bit 3 to 1
    PORTD.OUTSET = PIN3_bm;

    // Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
    PORTD.DIRSET = PIN3_bm;
    /* Set the RxD pin as an input*/
    PORTD.DIRCLR = PIN2_bm;

    // Set baud rate & frame format
    USARTD0.BAUDCTRLB = 0x00;
    USARTD0.BAUDCTRLA = 0xCF;

    // Set mode of operation
    USARTD0.CTRLA = 0x10;                       // enable low level interrupts
    USARTD0.CTRLC = 0x03;                       // async, no parity, 8 bit data, 1 stop bit

    // Enable transmitter and receiver
    USARTD0.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);
}

