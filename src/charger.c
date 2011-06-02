/**
 * @file   charger.c
 * @author Henry Jenkins
 * @date   20110609
 */

#include "charger.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define CHARGER_PORT        PORTB
#define CHARGER_STATUS_PIN  _BV(2)

static charger_callback __charger_callback = NULL;

ISR(PORTB_INT0_vect) {
    //check that the callback function has been set
    if (__charger_calback) {
        __charger_callback(charger_read());
    }
}

void charger_init(){
    /*
     * set up interupt
     */
    CHARGER_PORT.INTCTRL = 0x02;
    CHARGER_PORT.INT0MASK = _BV(2);
}

bool charger_read() {
    return !(CHARGER_PORT.IN & CHARGER_STATUS_PIN);
}

void charger_set_callback(charger_callback callback) {
    __charger_state = callback;
}

