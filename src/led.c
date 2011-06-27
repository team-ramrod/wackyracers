#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "debug.h"

#define LED_VALUE_PINS ( _BV(0) | _BV(1) | _BV(2) | _BV(3) )
#define LED_ENABLE_PIN   _BV(6)
#define LED_SELECT_LEFT  _BV(5)
#define LED_SELECT_RIGHT _BV(7)

static struct {
    uint8_t value;
    uint8_t enabled;
    uint8_t selected;
} __left_display, __right_display;

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
        __left_display.value   = number;
        __left_display.enabled = LED_ENABLE_PIN;
    } else {
        __left_display.value   = 0x00;
        __left_display.enabled = 0x00;
    }
}

void led_display_right(uint8_t number) {
    if (number < 0x10) {
        __right_display.value   = number;
        __right_display.enabled = LED_ENABLE_PIN;
    } else {
        __right_display.value   = 0x00;
        __right_display.enabled = 0x00;
    }
}
