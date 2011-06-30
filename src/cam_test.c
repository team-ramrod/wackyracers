#include "common.h"
#include "uart_comms.h"
#include "camera.h"
#include "led.h"
#include "clock.h"

#include "debug.h"

ISR(BADISR_vect) {
    ERROR("test-cam", "Bad vector encountered, PMIC.STATUS = [%x]", PMIC.STATUS);
    led_display_left(0);
    led_display_right(0);
}

char input;

int main(int argc, char *argv[]) {

    clock_init();
    led_init();
    uart_init();

    //interrupt_init();

    led_display(17);
    
    camera_reset();
    camera_snap();
    uint32_t filesize = camera_get_filesize();
    camera_get_block(0, 16, NULL); 
    camera_stop_image();

    while (1) {

        _delay_ms(8000);
        
    }
    return 0;	
}

//sends bluetooth command to motor board
ISR(INTERRUPT_BT)
{
    //blue_read_bluetooth(&stdio_blue, &stdio_to_motor_board, &stdio_cam);
}

//sends camera data to bluetooth
// ISR(INTERRUPT_CAM)
// {
//     uint8_t input = fgetc(stream_cam);
//     fprintf(stream_bt, "%i", input);
// }

ISR(INTERRUPT_BOARD)
{
    uint8_t input = fgetc(stream_board);
    fprintf(stream_board, "%i", input);
    led_display(input);
}
