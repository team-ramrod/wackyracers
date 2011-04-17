#ifndef __spid_h
#define __spid_h

#include "common.h"

#pragma interrupt_handler spid_stc_isr:iv_SPID_INT //UNTESTED iv_SPI_STC

void spid_init(void);
void write_spid(char byte);
unsigned char read_spid(void);
#endif

