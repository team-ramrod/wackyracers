#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ir.h"
#include "debug.h"

#define LED_VALUE_PINS ( _BV(0) | _BV(1) | _BV(2) | _BV(3) )
#define LED_ENABLE_PIN   _BV(6)
#define LED_SELECT_LEFT  _BV(5)
#define LED_SELECT_RIGHT _BV(7)
#define DEBOUNCE_PERIOD 30

static struct {
    uint8_t value;
    uint8_t enabled;
    uint8_t selected;
} __left_display, __right_display;

static int debounce_counter = 0;

ISR(LED_TC_OVF_vect, ISR_NOBLOCK) {
    static enum { __L_D, __R_D } __current = __L_D;

    switch (__current) {
        case __L_D:
            __current = __R_D;
            LED_PORT.OUT = __right_display.value
                         | __right_display.enabled
                         | __right_display.selected;
            break;
        case __R_D:
            __current = __L_D;
            LED_PORT.OUT = __left_display.value
                         | __left_display.enabled
                         | __left_display.selected;
            break;
    }

    // IR debounce code since we ran out of timers
    if (debounce_counter++ > DEBOUNCE_PERIOD) {
        ir_debounced();
        debounce_counter = 0;
    }
}

void clear_debounce_counter() {
    debounce_counter = 0;
}

void led_init() {
    DEBUG("led", "Starting initialization.");
    // Set the pre-scaler to 1024.
    // CLK freq is now 32 MHz / 1024 = 31250 Hz.
    LED_TC.CTRLA = 0x07;

    // Set the period of the interrupts.
    // Period of 260 = 31250 / 260 ~= 120 Hz.
    LED_TC.PER = 0x0104;

    LED_PORT.DIRSET =  LED_VALUE_PINS
                     | LED_ENABLE_PIN
                     | LED_SELECT_LEFT
                     | LED_SELECT_RIGHT;

    // Set the default values for the structs.
    __left_display.value     = 0x00;
    __left_display.enabled   = 0x00;
    __left_display.selected  = LED_SELECT_LEFT;

    __right_display.value    = 0x00;
    __right_display.enabled  = 0x00;
    __right_display.selected = LED_SELECT_RIGHT;

    // Enable the interrupt at a low level.
    LED_TC.INTCTRLA = 0x01;
    DEBUG("led", "Finished initialization.");
}

void led_display_left(uint8_t number) {
    if (number < 0x10) {
        VERBOSE("led", "Displaying [%i] on the left display.", number);
        __left_display.value   = number;
        __left_display.enabled = LED_ENABLE_PIN;
    } else {
        VERBOSE("led", "Turning left display off.");
        __left_display.value   = 0x00;
        __left_display.enabled = 0x00;
    }
}

void led_display_right(uint8_t number) {
    if (number < 0x10) {
        VERBOSE("led", "Displaying [%i] on the right display.", number);
        __right_display.value   = number;
        __right_display.enabled = LED_ENABLE_PIN;
    } else {
        VERBOSE("led", "Turning right display off.");
        __right_display.value   = 0x00;
        __right_display.enabled = 0x00;
    }
}
