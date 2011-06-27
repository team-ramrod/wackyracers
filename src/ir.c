#include "ir.h"

#include <avr/interrupt.h>

static cmd_t last_cmd = CMD_NONE;

void ir_init(void) {
	//timer init
	IR_TC.CTRLA = 0x06; // clk/256
	IR_TC.INTCTRLA = 0x02;
}

cmd_t ir_get_cmd(void) {
    // Please read the header file for what this should return! it should hold
    // no state :}
	cmd_t tmp_cmd = last_cmd;
	last_cmd = CMD_NONE;
	return tmp_cmd;
}

uint16_t get_time(void){
	return IR_TC.CNT;
}

void ir_set_callback(ir_callback callback) {
    //TODO: Implement this
}

EMPTY_INTERRUPT( IR_TC_OVF_vect )
