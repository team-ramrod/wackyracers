#include "common.h"
#include "uart_comms.h"
#include "uart_common.h"
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

    led_display(64);
    fprintf(stream_board, "Welcome to camera test.\n\n");
    camera_reset();
    fprintf(stream_board, "Camera has reset.\n\n");
    //camera_set_baudrate(9600);
    //uart_set_baudrate(stream_cam, 9600);
    camera_snap();
    fprintf(stream_board, "Camera has snapped.\n\n");
    
    uint16_t filesize = camera_get_filesize();
    fprintf(stream_cam, "finished, filesize is %d\n", filesize);
    /* Get a block */
    camera_get_block(0, 16);
    camera_stop_image();

    while (1) {

        fprintf(stream_board, "HELLO\n");
        
        _delay_ms(3000);
        
    }
    return 0;   
}
