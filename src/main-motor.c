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
#include "charger.h"
#include "motor_controller.h"
#include "led.h"

#include <avr/interrupt.h>

ISR(BADISR_vect) {
    led_display(0xEB);
}

typedef enum {BT, IR} controller_t;
typedef enum {OFF, LEFT, RIGHT} y_state_t;
typedef enum {OFF, FWD, BACK} x_state_t;

int main(int argc, char *argv[]) {
    charger_init();
    motor_controller_init();
    led_init();

    x_state_t x_state = OFF;
    y_state_t y_state = OFF;
    controller_t controller = IR;

    uint8_t num = 0;

    while(1) {
        cmd_t latest_cmd, ir_cmd, bt_cmd;
        ir_cmd = ir_get_cmd();
        bt_cmd = CMD_NONE;// cam_get_cmd(); // TODO
        if (controller == BT) {
            if (ir_cmd == ASSUME_CTRL) {
                controller = IR;
                continue;
            } else {
                latest_cmd = bt_cmd;
            }
        } else {
            if (bt_cmd == ASSUME_CTRL) {
                controller = BT;
                continue;
            } else {
                latest_cmd = ir_cmd;
            }
        }

        switch(controller) {
            case BT:
                if (ir_get_cmd() == ASSUME_CTRL) {
                    controller = IR;
                }
                break;
            case IR:
                if (blue_get_cmd() == ASSUME_CTRL) { //TODO: camera_board_get_cmd(); or something
                    controller = IR;
                }
                break;
        /}

    }

    return 0;	
}
