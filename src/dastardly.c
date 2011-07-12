#include "dastardly.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "debug.h"

#define IR1_PWM DASTARDLY_IR1_TC.CCA
#define IR2_PWM DASTARDLY_IR2_TC.CCA

void dastardly_init(){
    DEBUG("dastardly", "Started initialization.");

    // Set the pre-scaler to clk/1
    DASTARDLY_IR1_TC.CTRLA = 0x01;
    DASTARDLY_IR2_TC.CTRLA = 0x01;
    
    // Set PE0 pin required for PWM to be output.
    PORTE.DIRSET = 0x01;

    // Set PC0 pin required for PWM to be output.
    PORTC.DIRSET = 0x01;

    // Set pwm output to override the pins
    // and set to single slope PWM mode.
    DASTARDLY_IR1_TC.CTRLB = 0x10 | 0x03;
    DASTARDLY_IR2_TC.CTRLB = 0x10 | 0x03;

    // Set the period of the output.
    // 8 kHz at 32 MHz clock = period of 4096 - 1 = 0x0999.
    DASTARDLY_IR1_TC.PER = 880;
    DASTARDLY_IR2_TC.PER = 1000;

    dastardly_ir_start();

    DEBUG("dastardly", "Finished initialization.");
}

void dastardly_ir_start(){
    //set 50% duty cycle
    IR1_PWM = 400;
    IR2_PWM = 500;
}

void dastardly_ir_stop(){
    //set 0% duty cycle
    IR1_PWM = 0x0000;
    IR2_PWM = 0x0000;
}

