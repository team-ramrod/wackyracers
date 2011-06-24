/** @file   commander.c
 *  @author Henry Jenkins
 *  @date   Created: 23 June 2011
 *
 *
 *
 *  Status: Untested
*/
#include "charger.h"
#define ENABLE_UART_MOTOR_CALLBACK
#include "uart.h"
#include "common.h"

void commander_init() {
    charger_init();
//void charger_set_callback(charger_callback);
}

cmd_t get_cmd() {
    bool charger_pluged_in = charger_read();
    cmd_t cmd;
    cmd = CMD_NONE;
    return cmd;
}

// This is the callback from the uart from the cam_board
INTERRUPT(INTERRUPT_CAM) {
    //We need to do something here!!
}
