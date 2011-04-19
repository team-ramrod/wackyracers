/** @file   main-cam.c
 *  @author Simon Richards ( Sucks )
 *  @date   Created: 19 April 2011
 *
 *	Initialisation calls.
 *
 *  Status: Untested
*/
#include "common.h"


int main(int argc, char *argv[]) {
//    set_sleep_state(A_SLEEP_STATE_THAT_WORKS);
    bluetooth_init();
    camera_init();
    sei();        
    while(1) {
        sleep();
    }
    return 0;	
}
