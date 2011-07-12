/** @file   main-cam.c
 *  @author Simon Richards
 */
#include "common.h"
#include "uart_comms.h"
#include "led.h"
#include "clock.h"
#include "dastardly.h"

#include <avr/interrupt.h>
#include <util/atomic.h>

#include "debug.h"

// Hack to get the leds working
void ir_debounced() {
}

static int16_t buffer_read();
static void buffer_write(uint8_t);
static void send_to_board();
static void send_to_cam(uint8_t);
void main_loop();

int main(int argc, char *argv[]) {
    clock_init();

    led_init();
    uart_init();
    dastardly_init();

    interrupt_init();

    PORTC.DIRSET = _BV(1);
    PORTC.OUTSET = _BV(1);

    main_loop();

    return 0;
}

void main_loop() {
    static enum { FIRST, SECOND, THIRD } state = FIRST;
    static uint8_t count = 2,
                   full_count = 2;
    int16_t c;

    while(1) {
        c = getc(stream_bt);
        buffer_write(c);

        switch (state) {
            case FIRST:
                if (--count == 0) {
                    if (c == 0) {
                        state = SECOND;
                        count = 2;
                        full_count += 2;
                    } else {
                        send_to_board();
                        state = FIRST;
                        count = 2;
                        full_count = 2;
                    }
                }
                break;
            case SECOND:
                if (--count == 0) {
                    if (c == 0) {
                        send_to_cam(full_count);
                        state = FIRST;
                        full_count = count = 2;
                    } else {
                        state = THIRD;
                        count = c;
                        full_count += c;
                    }
                }
                break;
            case THIRD:
                if (--count == 0) {
                    send_to_cam(full_count);
                    state = FIRST;
                    count = 2;
                    full_count = 2;
                }
                break;
        }
    }
}

#define BUFFER_SIZE 30
#define increment(pos) ((pos + 1) % BUFFER_SIZE)

uint8_t buffer[BUFFER_SIZE];
volatile uint8_t read_position = 0, write_position = 0;

static void buffer_write(uint8_t val) {
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
        if (read_position == increment(write_position)) {
            read_position = increment(read_position);
        }
        buffer[write_position] = val;
        write_position = increment(write_position);
    }
}

static int16_t buffer_read() {
    int16_t tmp = -1;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
        if (read_position != write_position) {
            tmp = buffer[read_position];
            read_position = increment(read_position);
        }
    }

    return tmp;
}

static void send_to_board() {
    uint8_t c = buffer_read();
    switch(c) {
        case CMD_CAM_OFF:
            PORTC.OUTCLR = _BV(1);
            break;
        case CMD_CAM_ON:
            PORTC.OUTSET = _BV(1);
            break;
        case CMD_DASTARDLY_IR_OFF:
            dastardly_ir_stop();
            break;
        case CMD_DASTARDLY_IR_ON:
            dastardly_ir_start();
            break;
        default:
            putc(c, stream_board);
            break;
    }
    buffer_read();  // Discard a character
}

static void send_to_cam(uint8_t count) {
    while(count-- > 0) {
        putc(buffer_read(), stream_cam);
    }
}

ISR(BADISR_vect) { led_display(0x09); }
ISR(INTERRUPT_CAM) { putc(getc(stream_cam), stream_bt); }
EMPTY_INTERRUPT( INTERRUPT_BT )
EMPTY_INTERRUPT( INTERRUPT_BOARD )
EMPTY_INTERRUPT( INTERRUPT_DEBUG )
