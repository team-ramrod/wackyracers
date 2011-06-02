/** @file   main-motor.c
 *  @author Simon Richards
 *  @date   Created: 19 April 2011
 *
 *	Initiation calls and main loop.
 *
 *  
 *  Status: Untested
*/
#include "common.h"
#include "charger.h"
#include "motor_controller.h"
#include "led.h"

#include <avr/interrupt.h>

ISR(BADISR_vect) {
    led_display(0xEB);
}

int main(int argc, char *argv[]) {
    charger_init();
    motor_controller_init();
    led_init();

    uint8_t num = 0;

    while(1) {
        led_display(num++);
    }

    return 0;	
}
