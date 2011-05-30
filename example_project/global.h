
#define max_str_length 128

extern struct
{
 float gyro_x;
 float gyro_y;
 int16_t compass_x;
 int16_t compass_y;
 int16_t compass_z;
 float battery_voltage;      
 float generator_voltage_1;  
 float generator_voltage_2;  
 float acceleration_z;       
 float acceleration_y;      
 float acceleration_x;      
 float water_pressure;    // WAVE   
 float load_current_1;       
 float load_current_2;       
 float hydraulic_pressure_1; 
 float hydraulic_pressure_2;
 float arm_angle;
} parameters;
extern struct
{
 struct
 {
  unsigned recall_page: 1,
		   write: 1,
		   erase: 1,
		   recording: 1,
		   startup: 1;
 } flags;
 struct
 {
  unsigned
     gyro: 1,
     compass: 1,
     battery_voltage: 1,
     generator_voltage: 1,
		   load_current: 1,
		   acceleration: 1,
		   water_pressure: 1,
		   hydraulic_pressure: 1,
		   arm_angle: 1,
		   relay: 1,
		   gps: 1;
 } parameters;
 unsigned int buffer;
 unsigned int page;
 unsigned int recall_page;
 unsigned int pages;
 unsigned int delay;
 unsigned int gprs_pages_per_email;
 unsigned int gprs_total_pages;
 unsigned char gprs_no_of_emails;
 unsigned char gprs_hours;
} dataflash;
extern struct
{
 unsigned close_1: 1,
          close_2: 1;
} control;
extern struct
{
 long joules;
 float peak_kw;
 long sample_joules;
 long samples;
} energy;
extern struct
{
 char ok;
 char recall;
 unsigned char delay;
 unsigned long sector;
 unsigned long start_sector;
 unsigned long finish_sector;
} sd;
extern struct
{
 struct
 {
  char cell_number[21];
  char msg[161];
 } gsm;
 struct
 {
  char update;
  char msg[max_str_length];
 } gps;
 struct
 {
  unsigned pwr_on: 1,
           connect: 1,
		   idle: 1;
 } flags;
 char str[max_str_length];
 char msg[max_str_length];
} gprs;
extern struct
{
 unsigned char hour;
 unsigned char minute;
 unsigned char second;
 unsigned char tenth_second;
 unsigned char day;
 unsigned char month;
 unsigned char year;
} date_time;
extern struct
{
 unsigned long first_data_sector;
 unsigned long root_cluster;
 unsigned long total_clusters;
 unsigned int bytes_per_sector;
 unsigned int sector_per_cluster;
 unsigned int reserved_sector_count;
 unsigned long unused_sectors;
 unsigned long append_file_sector;
 unsigned long append_file_location;
 unsigned long file_size;
 unsigned long append_start_cluster;
 unsigned long start_block;
 unsigned long total_blocks; 
 unsigned char sdhc_flag;
 unsigned char card_type;
 char buffer_fat32[512];
 char memory_string[19];
 char tmp_buffer_fat32[512];
} file;

typedef struct {
    uint8_t flag;
    void (*function)(void);
} interrupt_t;

//pointer function 
extern void (*gprs_ptr)(void);


// Interrupt Control
extern interrupt_t interrupts[];
extern uint8_t num_interrupts;


// ex-local variables from gprs.h
extern struct
{
 int power_down;
 int gsm_gps;
 int gsm_send;
 int gsm_inbox;
 int email_send;
} gprs_index;
extern char try_alternative_email_address;
extern __flash char gprs_text[][55];
