/**
 * @file   test.c
 * @author Wim Looman
 * @date   20110606
 *
 * Flashes an LED on PB0 on and off.
 */

#include <avr/io.h>
#include <util/delay.h>

int main(int argc, char *argv[]) {
    PORTB.DIRSET = 0x01;
    
    while (1) {
        PORTB.OUTTGL = 0x01;
        _delay_ms(500.0);
    }
}
