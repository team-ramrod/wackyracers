#ifndef bluetooth_h
#define bluetooth_h

#include "common.h"

//initilizes bluetooth and sets up uarts it will communicate with
void blue_init(void);

//reads the bluetooth and outputs the drive command to the motor board
void blue_read_bluetooth (FILE *stdio_blue, FILE *stdio_motor, FILE *stdio_cam);

bool blue_send_byte(uint8_t byte);


#endif
