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

struct _motor left_motor;

int main(int argc, char *argv[]) {
    motor_init(&left_motor, LEFT);

    while(1) {
        
    }

    return 0;	
}
