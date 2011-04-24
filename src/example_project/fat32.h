#ifndef _fat32_h_
#define _fat32_h_

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "ports.h"

//Attribute definitions for file/directory
#define attr_read_only 0x01
#define attr_hidden 0x02
#define attr_system 0x04
#define attr_volume_id 0x08
#define attr_directory 0x10
#define attr_archive 0x20
#define attr_long_name 0x0f

#define fat32_dir_entry_size 0x32
#define fat32_empty 0x00
#define fat32_deleted 0xe5
#define fat32_get 0
#define fat32_set 1
#define fat32_read 0
#define fat32_verify 1
#define fat32_add 0
#define fat32_remove 1
#define fat32_low 0
#define fat32_high 1	
#define fat32_total_free 1
#define fat32_next_free 2
#define fat32_get_list 0
#define fat32_get_file 1
#define fat32_delete 2
#define fat32_eof 0x0fffffff

//external prototypes
extern unsigned char sd_init(void);
extern unsigned char sd_read_single_block(unsigned long start_block);
extern unsigned char sd_write_single_block(unsigned long start_block);
extern void uart0_write(char *ptr);



//global flag to keep track of free cluster count updating in FSinfo sector
static unsigned char free_cluster_count_updated;

//local prototypes
void fat32_init(void);
unsigned char get_boot_sector_data(void);
unsigned long get_first_sector(unsigned long cluster_number);
unsigned long get_set_free_cluster(unsigned char tot_or_next, unsigned char get_set, unsigned long fs_entry);
struct directory* find_files(unsigned char flag, unsigned char *filename);
unsigned long get_set_next_cluster (unsigned long cluster_number,unsigned char get_set,unsigned long cluster_entry);
unsigned char read_file(unsigned char flag, unsigned char *filename);
unsigned char convert_filename(unsigned char *filename);
void append_file(void);
unsigned long search_next_free_cluster(unsigned long start_cluster);
void memory_statistics(void);
void display_memory(unsigned char flag, unsigned long memory);
void delete_file(unsigned char *filename);
void free_memory_update(unsigned char flag, unsigned long size);
void sd_write_interrupt(void);

#endif
