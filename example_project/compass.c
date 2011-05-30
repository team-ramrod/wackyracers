#include "common.h"

#define HMC5843 0x3c
#define config_register 0
#define data_register   3
#define ready_bit       0x01

void compass_init(void)
{
 uint8_t data[3] = {0x18, 0x20, 0x00};
 twic_address(HMC5843, config_register);
 twic_write(HMC5843, data, 3);
 // This leaves the HMC permanently looping through the correct registers
 twic_address(HMC5843, data_register); 
}

void compass_read(void)
{
 uint8_t data[6];
 twic_read(HMC5843, data, 6);
 parameters.compass_x = data[0] << 8 | data[1];
 parameters.compass_y = data[2] << 8 | data[3];
 parameters.compass_z = data[4] << 8 | data[5]; 
}
