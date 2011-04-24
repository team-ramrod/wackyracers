#include "common.h"
uint8_t u1;

//local variables
static char rx1_msg[21];
static char *tx1_ptr;
static char tx1_fifo;
//UART1 initialize
// desired baud rate:9600
// actual baud rate:9615 (0.2%)
// char size: 8 bit
// parity: Disabled
void usartc1_init(void)
{
 USARTC1.DATA = 0x00;
 USARTC1.CTRLA = 0x2A;
 USARTC1.CTRLB = 0x18;
 USARTC1.CTRLC = 0x03;
 USARTC1.BAUDCTRLA = 0xD0;
 USARTC1.BAUDCTRLB = 0x00;
 u1 = attach_interrupt(usartc1_interrupt);
}


void usartc1_rx_isr(void)
{
 //USART1 RX complete
 char ch;
 static signed char k;
 static char msg[21];
 
 //uart has received a character in UDR
 ch = USARTC1.DATA;
 
 if (k< 21)
  k++;
 
 switch (ch)
 {
  case ':' :
   k= 0;
   msg[k]= ch;
   break;
  
  case '\r' :
   msg[k]= ch;
   msg[k+1]= null;
   strcpy(rx1_msg, msg);
   interrupts[u1].flag = bit_true;
   break;
  
  case '\n' :
   k= -1;
   break;
   
  default :
   msg[k]= ch;
   break;
 }
}

void usartc1_interrupt(void)
{
 interrupts[u1].flag = bit_false;
}

void usartc1_tx_isr(void)
{
 //USART1, Tx Complete
 if (*tx1_ptr!= 0x00)
 {
  USARTC1.DATA = (*tx1_ptr);
  tx1_ptr++;
 }
 else
 {
  tx1_fifo= false;
  //set_rx1_mode;
 }
}

void usartc1_write(char *ptr) //feed string into serial port
{
 static char tx1_msg[21];
 //set_tx1_mode;
 
 if (tx1_fifo) //append if already transmitting
  strcat(tx1_msg, ptr);
 else
 {
  strcpy(tx1_msg, ptr);
  tx1_ptr= tx1_msg;
  
  //send first char
  USARTC1.DATA = (*tx1_ptr);
  tx1_ptr++;
  
  tx1_fifo= true;
 }
}

void usartc1_putch(char byte)
{
 //set_tx1_mode;
 while (usartc1_busy);
 USARTC1.DATA = byte;
}
