#include "common.h"
#include "keyboard.h"
#include "uart_common.h"
#include <util/atomic.h>

#include "debug.h"

#define BUFFER_SIZE 30
#define increment(pos) ((pos + 1) % BUFFER_SIZE)

keyboard_callback __callback = NULL;
cmd_t __buffer[BUFFER_SIZE]  = { CMD_NONE };

volatile uint8_t __read_position  = 0,
                 __write_position = 0;

static void __buffer_write(cmd_t cmd) {
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
        if (__read_position == increment(__write_position)) {
            __read_position = increment(__read_position);
        }
        __buffer[__write_position] = cmd;
        __write_position = increment(__write_position);
    }
}

static cmd_t __buffer_read() {
    cmd_t tmp = CMD_NONE;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
        if (__read_position != __write_position) {
            tmp = __buffer[__read_position];
            __read_position = increment(__read_position);
        }
    }

    return tmp;
}

static inline cmd_t __translate(uint8_t c) {
    switch (c) {
        case 'q': return CMD_ASSUME_CTRL;
        case 'w': return CMD_FORWARD;
        case 'a': return CMD_LEFT;
        case 's': return CMD_BACK;
        case 'd': return CMD_RIGHT;
        case 'x': return CMD_STOP;
        default:  return CMD_NONE;
    }
}

void key_init() {
    __callback = __buffer_write;
}

cmd_t key_get_cmd() {
    return __buffer_read();
}

void key_set_callback(keyboard_callback callback) {
    __callback = callback;
}

void key_clear_callback() {
    __callback = __buffer_write;
}

ISR(INTERRUPT_DEBUG) {
    if (__callback != NULL) {
        uint8_t c = getc(stream_debug);
        cmd_t cmd = __translate(c);
        DEBUG("keyboard", "Received [%c] which translated to command [%i].", c, cmd);
        __callback(cmd);
    }
}
