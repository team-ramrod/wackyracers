#ifndef __spie_h
#define __spie_h

#include "common.h"

#pragma interrupt_handler spie_stc_isr:iv_SPID_INT //UNTESTED iv_SPI_STC


void spie_init(void);


void write_spie(char byte);

unsigned char read_spie(void);


#endif

