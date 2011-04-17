#ifndef __usartc1_h
#define __usartc1_h
#include "common.h"

#define tx1_busy !(USARTC1.STATUS & 0x20)
#define usartc1_busy tx1_busy
#pragma interrupt_handler usartc1_rx_isr:iv_USARTC1_RXC
#pragma interrupt_handler usartc1_tx_isr:iv_USARTC1_TXC

void usartc1_interrupt(void);
void usartc1_init(void);
void usartc1_write(char *ptr);


#endif

