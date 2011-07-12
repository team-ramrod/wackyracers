#include "common.h"
#include "uart_comms.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "clock.h"
#include "debug.h"


int main( void ) {
    clock_init();
    uart_init();
    interrupt_init();
    for (;;) {
    }
}

