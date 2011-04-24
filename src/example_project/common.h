#ifndef __common_h
#define __common_h

#include <AVRdef.h>
#include <ioxm128A3v.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include "global.h"
#include "mcu.h"
#include "timer.h"
#include "twic.h"
#include "rtc.h"
#include "compass.h"
#include "spie.h"
#include "spid.h"
#include "usartc1.h"
#include "usartd0.h"
#include "adc.h"
#include "ee.h"
#include "dataflash.h"
#include "gprs.h"
#include "fat32.h"
#include "compass.h"
#include <eeprom.h>

#define reset_watchdog asm("wdr")
#define sleep() asm("sleep")

#define true 0xff
#define false 0x00

#define bit_true 1
#define bit_false 0

#define null 0x00

#define bit(x) (1 << (x))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#define ch_null 0x00
#define ch_cr 0x0d
#define ch_ctrl_z 0x1a
#define ch_quote 0x22
#define ch_second 0x27
#define ch_fraction 0x22
#define ch_forward_slash 0x5c 
#define ch_single_quote 0x27


#define spid_busy !(SPID.STATUS & 0x80)
#define enable_spid SPID.CONTROL= 0x50
#define disable_spid SPID.CONTROL = 0x00

#define spie_busy !(SPIE.STATUS & 0x80)
#define enable_spie SPIE.CONTROL= 0x50
#define disable_spie SPIE.CONTROL = 0x00

#define max_str_length 128

#define min_pwm 5
#define max_pwm 250

#define _1second_delay 20

#define hex2dec(h) ((h)< 58 ? (h)-48 : (h)-55)

#define tx_newline {uart0_putch(0x0d); uart0_putch(0x0a);}

#define next(i,n) i = (i < n) ? i+1 : 0

// Configuration Options (User modifiable)
#define SENSOR_FREQUENCY 5.0 //Hz : not used
#define EMAIL_PERIOD 60.0 // Mins : not used
#define ADC_GAIN 0x01

 
#define K_AX    1663.3
#define K_AY    1645.875
#define K_AZ    1650
#define K_WAVE  0.0396
#define K_HYD1  0.0396
#define K_HYD2  0.0396
#define K_CRT1  0.01056
#define K_CRT2  0.01056
#define K_GX    0.0396
#define K_GY    0.0396
#define K_ANGLE 0.0396
#define K_VGEN1 0.0396
#define K_VGEN2 0.0396
#define K_VBAT  0.0396

#define O_AX	2.0328
#define O_AY	2.0262
#define O_AZ	2.0

#endif
