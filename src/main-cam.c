/** @file   main-cam.c
 *  @author Simon Richards
 *  @date   Created: 19 April 2011
 *
 */
#include "common.h"
#include "uart.h"
//#include "bluetooth.h"
#include "camera.h"
#include "led.h"
#include "clock.h"

char input;
uint8_t num = 0;
int main(int argc, char *argv[]) {
//    camera_init();
    led_init();
    clock_init();
    uart_init_cam_board();
    sei(); //enable interupts macro

    uint32_t i = 0;
    while(1) {
        if (i < 12 ) {
            led_display(i);
            fprintf(&stdio_to_motor_board, "%d\n", i);
            i++;
        }
    } //main while loop
    return 0;	
}

//sends bluetooth command to motor board
ISR(INTERRUPT_BLUE)
{
    //blue_read_bluetooth(&stdio_blue, &stdio_to_motor_board, &stdio_cam);
}

//sends camera data to bluetooth
ISR(INTERRUPT_CAM)
{
    uint8_t input = fgetc(&stdio_cam);
    fprintf(&stdio_blue, "%i", input);
}

ISR(INTERRUPT_MOTOR)
{
    uint8_t input = fgetc(&stdio_to_motor_board);
    fprintf(&stdio_to_motor_board, "%i", input);
    led_display(input);
}
