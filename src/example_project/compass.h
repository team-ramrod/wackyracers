#ifndef __compass_h
#define __compass_h

void compass_init(void);

uint8_t compass_is_ready(void);

void compass_read(void);
#endif
