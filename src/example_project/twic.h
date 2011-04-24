#ifndef __twic_h
#define __twic_h

void twic_init(void);
void twic_address(uint8_t device, uint8_t address);
void twic_write(uint8_t device, uint8_t * data, uint8_t size);
void twic_read(uint8_t device, uint8_t * data, uint8_t size);

#endif

