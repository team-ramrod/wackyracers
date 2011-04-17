#ifndef __ee_h
#define __ee_h

#include "common.h"
#include "emap.h"

#define ee_busy (NVM.STATUS & 0x80)

#define buffer_depth 20

//external prototypes

//local prototypes
unsigned char ee_read(unsigned int location);
static void load_to_ee(unsigned int location, unsigned char data);
char ee_write(unsigned int location, unsigned int data, unsigned char bytes, unsigned char real_time);
void ee_init(void);

#endif
