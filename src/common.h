#ifndef __common_h
#define __common_h

//#include <ioavr.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>


#define MOTOR_TC TCE0
#define LED_TC   TCD1

#define sleep() asm("sleep")

// Why not just use _BV ?
#define bit(x) (1 << (x))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#define next(i,n) i = (i < n) ? i+1 : 0

typedef enum {
    CMD_FORWARD   = 1,
    CMD_BACK      = 2,
    CMD_LEFT      = 4,
    CMD_RIGHT     = 8,
    CMD_STOP      = 16,
    CMD_TOGGGLE   = 32,
    CMD_GET_IMAGE = 64,
} cmd_t;

#endif
