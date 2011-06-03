/** @file   main-cam.c
 *  @author Simon Richards ( Sucks )
 *  @date   Created: 19 April 2011
 *
 *	Initialisation calls.
 *
 *  Status: Untested
*/
#include "common.h"
#include "bluetooth.h"
#include "camera.h"
#include "led.h"


int main(int argc, char *argv[]) {
//    set_sleep_state(A_SLEEP_STATE_THAT_WORKS);
//    bluetooth_init();
//    camera_init();
//    sei();        
    led_init();

    uint8_t num = 0;

    while(1) {
        led_display(num++);
    }

    return 0;	
}
