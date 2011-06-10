#include <stdio.h>
#include <avr/io.h>
#include "clock.h"
#include "led.h"
#include <util/delay.h>

static int uart_putchar(char c, FILE *stream);
static char uart_getchar(void);
static void uart_init (void);

static FILE mystdio = FDEV_SETUP_STREAM (uart_putchar, uart_getchar, _FDEV_SETUP_RW);
 

int main(int argc, char *argv[]) {
    uint8_t num = 0;

    clock_init();
    led_init();
    uart_init();

    stdout = &mystdio;

    PORTB.DIRSET = 0x01;

    while (1) {

        num = 0;
        while(num == 0)
        {
            num = uart_getchar();
        }
        printf("%i\n", num);


        led_display(num = (num + 1) % 100);
        PORTB.OUTTGL = 0x01;
        _delay_ms(500.0);
    }

    return 0;
}

static int uart_putchar (char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);
 
    // Wait for the transmit buffer to be empty
    while ( !( USARTE0.STATUS & USART_DREIF_bm) );
 
    // Put our character into the transmit buffer
    USARTE0.DATA = c; 
 
    return 0;
}

static char uart_getchar (void)//FILE *stream)
{
    unsigned char ret;

    while ( !( USARTE0.STATUS & USART_RXCIF_bm) );

    // Put our character into the transmit buffer
    ret = USARTE0.DATA;

    return ret;
}
 
 
// Init USART.  Transmit only (we're not receiving anything)
// We use USARTC0, transmit pin on PC3.
// Want 9600 baud. Have a 32 MHz clock. BSCALE = 0
// BSEL = ( 32000000 / (2^0 * 16*9600)) -1 = 12
// Fbaud = 32000000 / (2^0 * 16 * (12+1))  = 9615 bits/sec
static void uart_init (void)
{
    // Set the TxD pin high - set PORTC DIR register bit 3 to 1
    PORTE.OUTSET = PIN3_bm;
 
    // Set the TxD pin as an output - set PORTC OUT register bit 3 to 1
    PORTE.DIRSET = PIN3_bm;
    /* Set the RxD pin as an input*/
    PORTE.DIRCLR = PIN2_bm;
 
    // Set baud rate & frame format
    USARTE0.BAUDCTRLB = 0x00;                      // BSCALE = 0 as well
    USARTE0.BAUDCTRLA = 0xCF;
 
    // Set mode of operation
    USARTE0.CTRLA = 0;                          // no interrupts please
    USARTE0.CTRLC = 0x03;                       // async, no parity, 8 bit data, 1 stop bit
 
    // Enable transmitter and receiver
    USARTE0.CTRLB = (USART_TXEN_bm | USART_RXEN_bm);
}