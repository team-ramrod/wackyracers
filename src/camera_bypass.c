#include "common.h"
#include "uart_comms.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "clock.h"
#include "debug.h"

#define BUFFER_SIZE 30
#define increment(pos) ((pos + 1) % BUFFER_SIZE)

uint8_t __buffer[BUFFER_SIZE]  = { 0 };

volatile uint8_t __read_position  = 0,
                 __write_position = 0;

static void __buffer_write(uint8_t cmd) {
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
        if (__read_position == increment(__write_position)) {
            __read_position = increment(__read_position);
        }
        __buffer[__write_position] = cmd;
        __write_position = increment(__write_position);
    }
}

static int16_t __buffer_read() {
    int16_t tmp = -1;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
        if (__read_position != __write_position) {
            tmp = __buffer[__read_position];
            __read_position = increment(__read_position);
        }
    }

    return tmp;
}

static void __send_to_board() {
    int16_t c = 0;

    while ((c = __buffer_read()) != -1) {
        putc(c, stream_board);
    }
}

static void __send_to_cam() {
    int16_t c = 0;

    while ((c = __buffer_read()) != -1) {
        putc(c, stream_cam);
    }
}


ISR(INTERRUPT_CAM) {
    char c = getc(stream_cam);
    putc(c, stream_bt);
}

ISR(INTERRUPT_BT) {
    static enum { FIRST, SECOND, THIRD } state = FIRST;
    static uint8_t count = 2;

    char c = getc(stream_bt);

    switch (state) {
        case FIRST:
            __buffer_write(c);
            if (--count == 0 && c == 0) {
                state = SECOND;
                count = 0;
            } else {
                __send_to_board();
                state = FIRST;
                count = 2;
            }
            break;
        case SECOND:
            __buffer_write(c);
            if (--count == 0) {
                if (c == 0) {
                    __send_to_cam();
                    state = FIRST;
                    count = 2;
                } else {
                    state = THIRD;
                    count = c;
                }
            }
            break;
        case THIRD:
            __buffer_write(c);
            if (--count == 0) {
                __send_to_cam();
                state = FIRST;
                count = 2;
            }
            break;
    }
}

EMPTY_INTERRUPT(INTERRUPT_BOARD)

EMPTY_INTERRUPT(INTERRUPT_DEBUG)

int main( void ) {
    clock_init();
    uart_init();
    interrupt_init();
    for (;;) {
    }
}

