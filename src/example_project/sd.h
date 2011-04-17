#ifndef __sd_h
#define __sd_h

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "ports.h"
#include "emap.h"

#define sd_cs_assert PORTB &= ~0x10
#define sd_cs_deassert PORTB |= 0x10

// sd command codes
#define cmd_go_idle_state 0x00
#define cmd_send_op_cond 0x01
#define cmd_set_blocklen 0x10
#define cmd_read_single_block 0x11
#define cmd_write_single_block 0x18

//SD commands, many of these are not used here
#define go_idle_state 0x00
#define send_op_cond 1
#define send_if_cond 0x08
#define send_csd 9
#define stop_transmission 12
#define send_status 13
#define set_block_len 16
#define read_single_block 17
#define read_multiple_blocks 18
#define write_single_block 24
#define write_multiple_blocks 25
#define erase_block_start_addr 32
#define erase_block_end_addr 33
#define erase_selected_blocks 38
#define sd_send_op_cond 41
#define app_cmd 55
#define read_ocr 58
#define crc_on_off 59

#define on 1
#define off 0

//external prototypes
extern void write_spi(char byte);
extern unsigned char read_spi(void);
extern void uart0_write(char *ptr);
extern void uart0_putch(char byte);
extern char ee_write(unsigned int location, unsigned int data, unsigned char bytes, unsigned char real_time);

extern void uart0_putch(char byte);
extern void write_file(unsigned char *filename);
extern unsigned char get_boot_sector_data(void);
extern unsigned long get_first_sector(unsigned long cluster_number);
extern void memory_statistics(void);


//local prototype
unsigned char sd_send_command(unsigned char cmd, unsigned long arg);
static void sd_clock_and_release(void);
unsigned char sd_read_single_block(unsigned long start_block);
unsigned char sd_write_single_block(unsigned long start_block);
unsigned char sd_erase (unsigned long start_block, unsigned long total_blocks);

//local variables

#endif
