#ifndef __common_h
#define __common_h

#include <ioavr.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "camera.h"
#include "bluetooth.h"


#define sleep() asm("sleep")

#define true 0xff
#define false 0x00

#define bit(x) (1 << (x))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))


#define next(i,n) i = (i < n) ? i+1 : 0


#endif
