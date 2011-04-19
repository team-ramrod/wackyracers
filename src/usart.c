/** @file   usart.c
 *  @author Simon Richards ( Sucks )
 *  @date   Created: 19 April 2011
 *
 *
 *  Status: Untested
*/
#include "common.h"
#define BUFFER_LENGTH 10

typedef enum {
    C0,
    C1,
    D0,
    D1,
    E0,
    E1
} usart_id_t;
    

typedef struct {
    char data_in;
    char data_out;
    char buffer[BUFFER_LENGTH];
} usart_t;

void usart_init() {
    USARTC1.DATA = 0x00;
    USARTC1.CTRLA = 0x2A;
    USARTC1.CTRLB = 0x18;
    USARTC1.CTRLC = 0x03;
    USARTC1.BAUDCTRLA = 0xD0;
    USARTC1.BAUDCTRLB = 0x00;
}


void usartc1_rx_isr()
{
 //USART1 RX complete
 char ch;
 static signed char k;
 static char msg[21];
 
 //uart has received a character in UDR
 ch = USARTC1.DATA;
 
 if (k< 21)
  k++;
 
 switch (ch) {
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

void usartc1_tx_isr()
{
    //USART1, Tx Complete
    if (*tx1_ptr!= 0x00) {
        USARTC1.DATA = (*tx1_ptr);
        tx1_ptr++;
    } else {
        tx1_fifo= false;
        //set_rx1_mode;
    }
}

void usartc1_write(char *ptr) //feed string into serial port
{
    static char tx1_msg[21];
    //set_tx1_mode;

    if (tx1_fifo) { //append if already transmitting
        strcat(tx1_msg, ptr);
    } else {
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
