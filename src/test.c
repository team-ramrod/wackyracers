/**
 * @file   test.c
 * @author Wim Looman
 * @date   20110606
 *
 * Flashes an LED on PB0 on and off.
 */

#include "clock.h"
#include "led.h"

#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char *argv[]) {
    uint8_t num = 0;

    clock_init();
    led_init();

    PORTB.DIRSET = 0x01;
    
    while (1) {
        led_display(num = (num + 1) % 100);
        PORTB.OUTTGL = 0x01;
        _delay_ms(2000.0);
    }

    return 0;
}
