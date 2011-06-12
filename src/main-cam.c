/** @file   main-cam.c
 *  @author Simon Richards ( Sucks )
 *  @date   Created: 19 April 2011
 *
 *	Initialisation calls.
 *
 *  Status: Untested
*/
#include "common.h"
#include "uart_cam_board.h"
#include "bluetooth.h"
#include "camera.h"
#include "led.h"
#include "clock.h"

//sets up stdio for bluetooth
FILE stdio_blue = FDEV_SETUP_STREAM (uart_putchar_blue, uart_getchar_blue, _FDEV_SETUP_RW);

//sets up stdio for communicating with motor board
FILE stdio_motor = FDEV_SETUP_STREAM (uart_putchar_motor, uart_getchar_motor, _FDEV_SETUP_RW);

//sets up stdio for camera
FILE stdio_cam = FDEV_SETUP_STREAM (uart_putchar_cam, uart_getchar_cam, _FDEV_SETUP_RW);

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
    blue_read_bluetooth(&stdio_blue, &stdio_motor);
}
