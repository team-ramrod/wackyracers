/** @file   commander.c
 *  @author Henry Jenkins
 *  @date   Created: 23 June 2011
 *
 *
 *
 *  Status: Untested
*/
#define ENABLE_UART_MOTOR_CALLBACK

#include "charger.h"
#include "common.h"
#include "ir.h"
#include "uart_motor.h"

typedef enum {BT, IR} controller_t;


static volatile cmd_t current_cmd = CMD_NONE;
static controller_t controller = IR;
static bool charger_state;

/**
 * Just set the next command to stop, then rely on the other functions passing
 * back CMD_NONE when charger is plugged in.
 */
void charger_callback_function(bool plugged_in) {
    charger_state = plugged_in;
    if (plugged_in) {
        current_cmd = CMD_STOP;
    }
}
void uart_motor_board_callback_function(cmd_t command) {
    if (command == CMD_ASSUME_CTRL) {
        controller = BT;
    } else if (!charger_state && controller == BT) {
        current_cmd = command;
    }
    current_cmd = CMD_NONE;
}
void ir_callback_function(cmd_t command) {
    if (command == CMD_ASSUME_CTRL) {
        controller = IR;
    } else if (!charger_state && controller == IR) {
        current_cmd = command;
    }
    current_cmd = CMD_NONE;
}

void commander_init() {
    //charger init
    charger_init();
    charger_set_callback(&charger_callback_function);
    charger_state = charger_read();

    //uart init
    uart_init();
    uart_set_motor_board_callback(&uart_motor_board_callback_function);

    //ir init
    ir_init();
    ir_set_callback(&ir_callback_function);
}

cmd_t get_cmd() {
    cmd_t cmd_tmp = current_cmd;
    current_cmd = CMD_NONE;
    return cmd_tmp;
}

