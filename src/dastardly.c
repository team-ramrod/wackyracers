#include "dastardly.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "debug.h"

#define IR_PWM DASTARDLY_IR_TC.CCA

void dastardly_init(){
    DEBUG("dastardly", "Started initialization.");

    // Set the pre-scaler to clk/1
    DASTARDLY_IR_TC.CTRLA = 0x01;

    // Set PC0 pin required for PWM to be output.
    PORTC.DIRSET = 0x01;

    // Set the period of the output.
    // 8 kHz at 32 MHz clock = period of 4096 - 1 = 0x0999.
    MOTOR_TC.PER = 0x0999;

    dastardly_ir_stop();

    DEBUG("dastardly", "Finished initialization.");
}

void dastardly_ir_start(){
    //set 50% duty cycle
    IR_PWM = 0x00FF;
}

void dastardly_ir_stop(){
    //set 0% duty cycle
    IR_PWM = 0x0000;
}

