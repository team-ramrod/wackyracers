#include "button.h"

#define PULLDOWN _BV(4)

static button_callback __power_callback = NULL;

void button_init() {
    // Set the output side high.
    PORTB.DIRSET = _BV(1);
    PORTB.OUTSET = _BV(1);

    // Set the input side on pulldown.
    PORTB.PIN3CTRL |= PULLDOWN;
}

bool button_read(button_t button) {
    switch (button) {
        case BUTTON_POWER: return !(PORTB.IN & _BV(3));
        default: return false;
    }
}

void button_set_callback(button_t button, button_callback callback) {
    switch (button) {
        case BUTTON_POWER: __power_callback = callback;
    }
}
