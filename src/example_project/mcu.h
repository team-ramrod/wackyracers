#ifndef micro_h
#define micro_h

#include "ports.h"

void port_init(void);


uint8_t attach_interrupt(void (*func)(void));

void interrupts_init(void);

void watchdog_init(void);

void clock_init(void);

void sleep_until_interrupt(void);

#endif
