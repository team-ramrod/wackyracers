/** @file   main-cam.c
 *  @author Simon Richards ( Sucks )
 *  @date   Created: 19 April 2011
 *
 *	Initialisation calls.
 *
 *  Status: Untested
*/
#include "common.h"
#include "uart.h"
#include "bluetooth.h"
#include "camera.h"
#include "led.h"
#include "clock.h"

char input;
uint8_t num = 0;
int main(int argc, char *argv[]) {
//    set_sleep_state(A_SLEEP_STATE_THAT_WORKS);
//    camera_init();
        
    led_init();
    clock_init();
    uart_init_cam_board();
    sei();

    

    while(1) {
        led_display(num++);
        _delay_ms(500.0);
    }

    return 0;	
}

ISR(INTERRUPT_MOTOR)
{
    num = num+ 10;
    blue_read_bluetooth(&stdio_blue, &stdio_to_motor_board);
}
