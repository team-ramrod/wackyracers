#include "common.h"

#define NUM_DEVICES   2

#define I2C_TIMEOUT   200
#define READ          TWIC.MASTER.STATUS & 0x80
#define WRITE         TWIC.MASTER.STATUS & 0x40

#define ACK_START     TWIC.MASTER.CTRLC = 0x01
#define ACK_RECEIVE   TWIC.MASTER.CTRLC = 0x02
#define ACK_STOP      TWIC.MASTER.CTRLC = 0x03

#define NACK_START    TWIC.MASTER.CTRLC = 0x05
#define NACK_RECEIVE  TWIC.MASTER.CTRLC = 0x06
#define NACK_STOP     TWIC.MASTER.CTRLC = 0x07

#define ADDRESS       TWIC.MASTER.ADDR
#define DATA          TWIC.MASTER.DATA

static void wait_for(uint8_t action);
 
/**
 * Initialises the TWI on port C in master mode.
 */
void twic_init(void)
{
 TWIC.MASTER.CTRLA = 0xB8;
 TWIC.MASTER.CTRLB = 0x0C;
}

/**
 * Reads from 1 or more registers into a given array. 
 *
 * @param device The slave address byte corresponding to the desired device in read mode.
 * @param data An array of length size into which the device's registers will be saved
 * @param size Number of registers to read
 */
void twic_read(uint8_t device, uint8_t * data, uint8_t size)
{
 uint8_t i;
 ADDRESS = device | 0x01;
 wait_for(WRITE);
 for (i = 0; i < size; i++)
 {
  wait_for(READ);
  data[i] = DATA;
  if (i < size - 1)
   ACK_START;
  else
   NACK_STOP;
 }
}


/**
 * Writes to one or more registers from an array. 
 *
 * @param device The slave address bte corresponding to the desired device in write mode.
 * @param data An array of length size from which the device's registers will be read.
 * @param size Number of registers to write to.
 */
void twic_write(uint8_t device, uint8_t * data, uint8_t size)
{
  uint8_t i;
  ADDRESS = device;
  wait_for(WRITE);
  for (i = 0; i < size; i++)
  {
   DATA = data[i];
   wait_for(WRITE);
  }
  ACK_STOP;
}

/**
 * Sets the address pointer
 *
 * @param device The desired I2C device.
 * @param address The address you wish to start the foloowing read or write from
 */
void twic_address(uint8_t device, uint8_t address)
{
 twic_write(device, &address, 1);
}

/**
 * Private function that waits on writes or reads 
 *
 * @param action Use #define READ or #define WRITE.
 */
static void wait_for(uint8_t action)
{
 uint8_t i = 0;
 while (!action)
  if (i++ > I2C_TIMEOUT)
   return;
}

