#include "button.h"

#define PULLUP _BV(4)
#define NUM_BUTTONS 3

static struct button {
    button_callback  callback;
    PORT_t          *port;
    uint8_t          out_pin;
    uint8_t          in_pin;
} __buttons[NUM_BUTTONS];

static uint8_t __current = 0;

button_t button_init(button_config config) {
    if (__current >= NUM_BUTTONS) {
        return NULL;
    }

    button_t button = &__buttons[__current++];

    button->port    = config.port;
    button->out_pin = config.out_pin;
    button->in_pin  = config.in_pin;

    // Set the output side low.
    (*button->port).DIRSET = _BV(button->out_pin);
    (*button->port).OUTCLR = _BV(button->out_pin);

    // Set the input side on pullup.
    (&(*button->port).PIN0CTRL)[button->out_pin] |= PULLUP;

    return button;
}

bool button_read(button_t button) {
    return !((*button->port).IN & _BV(button->in_pin));
}

void button_set_callback(button_t button, button_callback callback) {
    button->callback = callback;
}
