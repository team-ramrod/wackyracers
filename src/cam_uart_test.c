/** @file   main-cam.c
 *  @author Simon Richards
 *  @date   Created: 19 April 2011
 *
 */
#include "common.h"
#include "uart_comms.h"
//#include "bluetooth.h"
#include "camera.h"
#include "led.h"
#include "clock.h"
#include "debug.h"

ISR(BADISR_vect) {
    ERROR("main-motor", "Bad vector encountered, PMIC.STATUS = [%x]", PMIC.STATUS);
    led_display_left(0);
    led_display_right(0);
}

EMPTY_INTERRUPT(INTERRUPT_BT)
EMPTY_INTERRUPT(INTERRUPT_DEBUG)

char input;
uint8_t num = 0;
int main(int argc, char *argv[]) {
    clock_init();
    led_init();
    uart_init();
    //cam_init();
    interrupt_init();

    uint8_t i = 0;
    while(1) {
        if (i < 12) {
            led_display(i);
            i++;
            fputc(i, stream_cam);
        }
    } //main while loop
    return 0;	
}

//sends camera data to bluetooth
ISR(INTERRUPT_CAM)
{
    uint8_t input = fgetc(stream_cam);
    //led_display(input);
    fputc(input, stream_board);
}

ISR(INTERRUPT_BOARD)
{
    uint8_t input = fgetc(stream_board);
    led_display(input);
    fputc(input, stream_cam);
}
