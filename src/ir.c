#include "ir.h"
#include <util/delay.h>
#include "led.h"
#include "uart_motor.h"
#include "debug.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

static cmd_t cmd = CMD_NONE;

uint8_t impulse_prev = 0;
uint8_t counter_impulse_1 = 0;
uint8_t flag_rx = 0;
uint8_t counter_impulse_0 = 0;
uint8_t impulse = 0;
uint16_t counter_ik = 0;
uint16_t code = 0;

static bool debouncing = false;

#define BUFFER_SIZE 30
#define increment(pos) ((pos + 1) % BUFFER_SIZE)

ir_callback __ir_callback = NULL;
cmd_t __ir_buffer[BUFFER_SIZE]  = { CMD_NONE };

volatile uint8_t __ir_read_position  = 0,
                 __ir_write_position = 0;

static void __ir_buffer_write(cmd_t cmd) {
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
        if (__ir_read_position == increment(__ir_write_position)) {
            __ir_read_position = increment(__ir_read_position);
        }
        __ir_buffer[__ir_write_position] = cmd;
        __ir_write_position = increment(__ir_write_position);
    }
}

static cmd_t __ir_buffer_read() {
    cmd_t tmp = CMD_NONE;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) {
        if (__ir_read_position != __ir_write_position) {
            tmp = __ir_buffer[__ir_read_position];
            __ir_read_position = increment(__ir_read_position);
        }
    }

    return tmp;
}

static inline cmd_t __translate(uint16_t c) {
    switch (c) {
        case 3586: return CMD_ASSUME_CTRL;
        case 4354: return CMD_ASSUME_CTRL;
        case 258: return CMD_FORWARD;
        case 1026: return CMD_LEFT;
        case 514: return CMD_BACK;
        case 770: return CMD_RIGHT;
        case 2306: return CMD_STOP;
        default:  return CMD_NONE;
    }
}

void ir_init(void) {
    //timer init
    IR_TC.CTRLA = 0x01; // clk/1
    IR_TC.INTCTRLA = 0x02; //sets overflow interrupt

    IR_TC.PER = 2963; //sets point at which timer overflows
    //2963 gives an overflow rate of 10.8 kHz

    __ir_callback = __ir_buffer_write;

    led_display(50);
}

uint16_t get_time(void) {
    return IR_TC.CNT;
}

cmd_t ir_get_cmd() {
    return __ir_buffer_read();
}

void ir_set_callback(ir_callback callback) {
    __ir_callback = callback;
}

void ir_clear_callback() {
    __ir_callback = __ir_buffer_write;
}

//IR interrupt based on code from http://www.scienceprog.com/ir-remote-control-signal-receiver-using-avr-microcontroller/
//this code contains a lot of magic numbers but it seems to work as long as interrupt occours at 10.8 kHz
ISR(IR_TC_OVF_vect) {

    //inverts signal
    if (PORTD.IN & bit(0)){
        impulse = 0;
    }
    else{
        impulse = 1;
    }

    //detects rising edge
    if ((impulse_prev == 0) && (impulse == 1)) {
        //reset counter
        counter_impulse_1 = 0;

        //if receving input
        if ((counter_impulse_0 > 3) && (flag_rx == 1) && (counter_impulse_0 < 20)) {

            //logical 1 received
            if (counter_impulse_0 > 16)
            {
                code |= (1 << counter_ik++);
            }
            //logical 0 received
            else
            {
                counter_ik++;
            }
        }
    }

    //counting positive pulses
    if ((impulse == 1) && (counter_impulse_1 < 35)){
            counter_impulse_1++;
    }

    //negative front
    if ((impulse_prev == 1) && (impulse == 0))
    {
        counter_impulse_0 = 0;

        //one long synchro pulse
        if (counter_impulse_1 > 30)
        {
            code = 0;
            counter_ik = 0;
            flag_rx = 1;
        }
    }

    //counting negative pulses
    if ((impulse == 0) && (flag_rx == 1)&(counter_impulse_1 < 150)){
        counter_impulse_0++;
    }

    //end of receive
    if ((counter_impulse_0 > 50) && (flag_rx == 1) && (impulse == 0))
    {
        if (!debouncing) { 
            cmd = __translate(code);
            DEBUG("ir", "Received [%i] which translated to command [%i].", code, cmd);
            __ir_callback(cmd);
            debouncing = true;
            clear_debounce_counter();
            LED_TC.CNT = 0;
        } else {
            DEBUG("ir", "Ignored [%i] which translated to command [%i].", code, cmd);
        }
        flag_rx = 0;
        counter_impulse_0 = 0;
        counter_impulse_1 = 0;
        counter_ik = 0;
        code = 0;
    }

    impulse_prev = impulse;
}

void ir_debounced() {
    debouncing = false;
}

