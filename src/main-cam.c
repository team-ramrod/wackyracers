/** @file   main-cam.c
 *  @author Simon Richards
 *  @date   Created: 19 April 2011
 *
 */
#include "common.h"
#include "uart_comms.h"
#include "bluetooth.h"
#include "camera.h"
#include "led.h"
#include "clock.h"

#include "debug.h"

ISR(BADISR_vect) {
    ERROR("main-cam", "Bad vector encountered, PMIC.STATUS = [%x]", PMIC.STATUS);
    led_display_left(0);
    led_display_right(0);
}

char input;
uint8_t num = 0;
int main(int argc, char *argv[]) {
    clock_init();

//    camera_init();
    led_init();
    uart_init();
    blue_init();

    interrupt_init();

    volatile uint16_t i = 0;
    led_display(67);
    while(1) {
    //    fprintf(stream_bt, "b");
        while(++i );
    } //main while loop
    return 0;	
}

//sends bluetooth command to motor board
ISR(INTERRUPT_BT)
{
    volatile static int i = 0;
    //blue_read_bluetooth(stream_bt, stream_board, stream_bt);
    char c = fgetc(stream_bt);
    fprintf(stream_board, "%c",c);
    //led_display(i++);
}
/*
//sends camera data to bluetooth
ISR(INTERRUPT_CAM)
{
    uint8_t input = fgetc(stream_cam);
    fprintf(stream_bt, "%i", input);
}
*/
ISR(INTERRUPT_BOARD)
{
    uint8_t input = fgetc(stream_board);
    fprintf(stream_board, "%i", input);
    led_display(input);
}
