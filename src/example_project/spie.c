#include "spie.h"

//SPIE initialize
// clock rate: 2000000hz
void spie_init(void)
{
 SPIE.CTRL = 0x50; //setup SPIC: Master mode, MSB first, SCK phase low, SCK idle low
}


void spie_stc_isr(void)
{
  //  SPIE Serial Transfer Complete 
}

void write_spie(char byte)
{
 unsigned int i= 0x1fff;
 
 SPIE.DATA = byte; 
 while (spie_busy && (--i)); //wait for data transfer to be completed
}

unsigned char read_spie(void)
{
 write_spie(0xff);
 return SPIE.DATA;
}
