#include "spid.h"
void spid_init(void)
{
 SPID.CTRL = 0x50; //setup SPID: Master mode, MSB first, SCK phase low, SCK idle low
}


void spid_stc_isr(void)
{
  //  SPID Serial Transfer Complete 
}

void write_spid(char byte)
{
 unsigned int i= 0x1fff;
 
 SPID.DATA = byte; 
 while (spid_busy && (--i)); //wait for data transfer to be completed
}

unsigned char read_spid(void)
{
 write_spid(0xff);
 return SPID.DATA;
}
