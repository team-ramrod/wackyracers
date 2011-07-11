#include "common.h"
#include "uart_comms.h"
#include <avr/interrupt.h>
#include "clock.h"
#include "debug.h"

ISR(INTERRUPT_CAM) {
    char c = getc(stream_cam);
    //fprintf(stream_debug, "c%x\n", c);
    putc(c, stream_debug);
}

ISR(INTERRUPT_DEBUG) {
    char c = getc(stream_debug);
    //fprintf(stream_debug, "b%x\n", c);
    putc(c, stream_debug);
}

EMPTY_INTERRUPT(INTERRUPT_BOARD)

EMPTY_INTERRUPT(INTERRUPT_BT)

int main( void ) {
    clock_init();
    uart_init();
    interrupt_init();
    for (;;) {
    }
}

