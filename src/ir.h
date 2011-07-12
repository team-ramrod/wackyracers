/**
 * @file   ir.h
 * @author ???
 * @date   20110530
 */

#ifndef ir_h
#define ir_h

#include "common.h"

typedef void (*ir_callback) (cmd_t);

/**
 * Initilises the ir module
 */
void ir_init(void);


/**
 * Returns a command that has been recived by the IR module. if there was no
 * command sent, then this returns CMD_NONE.
 *
 * @return command from IR module
 */
cmd_t ir_get_cmd(void);

/**
 * Sets the callback to trigger when a command is recived by the IR
 *
 * @param callback The callback to trigger, the param is the cmd_t recived
 */
void ir_set_callback(ir_callback);


void ir_debounced();

#endif
