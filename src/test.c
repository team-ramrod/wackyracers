/**
 * @file   test.c
 * @author Wim Looman
 * @date   20110606
 *
 * Flashes an LED on PB0 on and off.
 */

#include "clock.h"
#include "led.h"
#include "button.h"

#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char *argv[]) {
    uint8_t num = 0;

    clock_init();
    led_init();

    button_config button_1_config = {&PORTB, 1, 3};
    button_t button_1 = button_init(button_1_config);

    PORTB.DIRSET = 0x01;
    
    while (!button_read(button_1)) {
        // Do nothing
    }

    while (1) {
        led_display(num = (num + 1) % 100);
        PORTB.OUTTGL = 0x01;
        _delay_ms(2000.0);
    }

    return 0;
}
