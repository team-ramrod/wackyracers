#ifndef __dataflash_h
#define __dataflash_h

#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "common.h"
#include "ports.h"
#include "emap.h"

#define at45db16_pages 4000

//10seconds timeout
#define timeout_limit 40

//defines for all opcodes

// buffer 1/2 write 
#define buffer_1_write 0x84
#define buffer_2_write 0x87

// buffer 1/2 read
#define buffer_1_read 0xd4
#define buffer_2_read 0xd6

// buffer 1/2 to main memory page program with built-in erase
#define b1_to_mm_page_prog_with_erase 0x83
#define b2_to_mm_page_prog_with_erase 0x86

// buffer 1/2 to main memory page program without built-in erase
//#define b1_to_mm_page_prog_without_erase 0x88
//#define b2_to_mm_page_prog_without_erase 0x89

// main memory page program through buffer 1/2
#define mm_page_prog_through_b1 0x82
#define mm_page_prog_through_b2 0x85
 
// main memory page to buffer 1/2 transfer
#define mm_page_to_b1_xfer 0x53
#define mm_page_to_b2_xfer 0x55

// DataFlash status register for reading density, compare status, and ready/busy status
#define status_register_read 0xd7

// main memory page read
#define main_memory_page_read 0x52

// erase 512 pages
#define block_erase 0x50

#define page_length 512
#define block_length 512

//external prototypes
//extern void uart0_write(char *ptr);
extern char ee_write(unsigned int location, unsigned int data, unsigned char bytes, unsigned char real_time);
extern void gprs_email_send(void);

//local prototypes
void dataflash_init(void);
static char dataflash_busy(void);
static unsigned char dataflash_status(void);
static void erasing(void);
static void write_to_flash(unsigned char flash_data);
static void recalling(void);
void next_page_to_next_buffer(unsigned char active_buffer, unsigned int page_counter);
static void active_buffer_to_port(unsigned char active_buffer);
void dataflash_interrupt(void);

//local variables

#endif
