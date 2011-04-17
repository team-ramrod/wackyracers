#ifndef __usartd0_h
#define __usartd0_h

#define tx_newline {uart0_putch(0x0d); uart0_putch(0x0a);}

//uart calls
#define tx0_busy !(USARTD0.STATUS & 0x20)

#pragma interrupt_handler usartd0_rx_isr:iv_USARTD0_RXC
#pragma interrupt_handler usartd0_tx_isr:iv_USARTD0_TXC


static char check_valid_command(void);
static unsigned int str2int(char *ptr, unsigned char start, unsigned char stop);
static void get_cell_number(void);
static char check_string_command(unsigned char i);
void usartd0_interrupt(void);
void usartd0_init(void);

#endif
