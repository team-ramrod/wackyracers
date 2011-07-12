#ifndef __common_h
#define __common_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>


#define MOTOR_TC        TCE0
#define LED_TC          TCD1
#define IR_TC           TCD0
#define SERVO_UPDATE_TC TCC0
#define DASTARDLY_IR_TC TCE0
#define SERVO_STROBE_TC TCC1
#define IR_DEBOUNCE_RTC RTC

#define LED_TC_OVF_vect   TCD1_OVF_vect
#define IR_TC_OVF_vect    TCD0_OVF_vect
#define SERVO_UPDATE_TC_OVF_vect TCC0_OVF_vect
#define SERVO_STROBE_TC_OVF_vect TCC1_OVF_vect
#define IR_DEBOUNCE_RTC_OVF_vect RTC_OVF_vect

#define LED_PORT          PORTA
#define MOTOR_ENABLE_PORT PORTD

#define sleep() asm("sleep")

#define bit(x) (1 << (x))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#define next(i,n) i = (i < n) ? i+1 : 0

typedef enum {
    CMD_NONE              = 0,
    CMD_FORWARD           = 1,
    CMD_BACK              = 2,
    CMD_DASTARDLY_IR_OFF  = 3,
    CMD_LEFT              = 4,
    CMD_DASTARDLY_IR_ON   = 5,
    CMD_RIGHT             = 8,
    CMD_STOP              = 16,
    CMD_ASSUME_CTRL       = 32,
    CMD_CAM_OFF           = 64,
    CMD_CAM_ON            = 128,
} cmd_t;

#endif
