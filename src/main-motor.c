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
#include "motor.h"
#include "led.h"

#include <avr/interrupt.h>

ISR(BADISR_vect) {
    led_display(0xEB);
}

int main(int argc, char *argv[]) {
    motor_init();
    led_init();

    while(1) {
        
    }

    return 0;	
}
