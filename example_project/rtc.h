#ifndef __rtc_h
#define __rtc_h

#define ds1307 0xd0

#define second_register 0
#define minute_register 1
#define hour_register 2
#define date_register 4
#define month_register 5
#define year_register 6

#define clock_halted 0x80

#define twi_attempts 1000


//local prototypes
static void wait_for_twic_interrupt(void);
static unsigned char time_correction(unsigned char temp);
static unsigned char adjust_time(unsigned char temp);
void get_date(void);
void get_time(void);
void set_date(void);
void set_time(void);
void update_time_and_date(void);
void rtc_init(void);
//local variables

#endif
