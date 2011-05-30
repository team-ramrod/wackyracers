#include "led.h"
#include <avr/io.h>

#define LED_VALUE_PINS ( _BV(1) | _BV(2) | _BV(3) | _BV(4) )
#define LED_ENABLE_PIN _BV(6)
#define LED_SELECT_PIN _BV(7)

static struct {
    uint8_t value;
    uint8_t enabled;
    uint8_t selected;
} __left_display, __right_display;

static enum { __L_D, __R_D } __current;

void __callback() {
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
    // Set the pre-scaler to 1024.
    // CLK freq is now 32 MHz / 1024 = 31250 Hz.
    LED_TC.CTRLA = 0x07;

    // Set the period of the interrupts.
    // Period of 260 = 31250 / 260 ~= 120 Hz.
    LED_TC.PER = 0x0104;

    // Enable the interrupt and assign the callback.
    // LED_TC.INTCTRLA = 0x01 ? 0x02 ? 0x03;

    LED_PORT.DIRSET =  LED_VALUE_PINS
                     | LED_ENABLE_PIN
                     | LED_SELECT_PIN;

    // Set the default values for the structs.
    __left_display.value     = 0x00;
    __left_display.enabled   = 0x00;
    __left_display.selected  = LED_SELECT_PIN;
    __right_display.value    = 0x00;
    __right_display.enabled  = 0x00;
    __right_display.selected = 0x00;
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
