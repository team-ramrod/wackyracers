#ifndef bluetooth_h
#define bluetooth_h

#include "common.h"

//initilizes bluetooth and sets up uarts it will communicate with
void blue_init(void);

//reads the bluetooth and outputs the drive command to the motor board
inline void blue_read_bluetooth (char *string, int length);

bool blue_send_byte(uint8_t byte);


#endif
