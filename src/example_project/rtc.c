#include "common.h"

void update_time_and_date(void)
{
 static unsigned char k, second;
 //measure to the tenth of a second
 get_date();
 get_time();
 if (date_time.second!= second)
  k= 0;
 date_time.tenth_second= k;
 second= date_time.second; 
}

void rtc_init(void)
{
 date_time.day= 12;
 date_time.month= 1;
 date_time.year= 11;
 set_date();
  
 date_time.second= 0;
 date_time.minute= 48;
 date_time.hour= 9;
 set_time();
}


void get_time(void)
{
 uint8_t data[3];
 twic_address(ds1307, second_register);
 twic_read(ds1307, data, 3);
 
 date_time.second = time_correction(data[0]);
 date_time.minute = time_correction(data[1]);
 date_time.hour   = time_correction(data[2]);
}

void get_date(void)
{
 uint8_t data[3];
 twic_address(ds1307, date_register);
 twic_read(ds1307, data, 3);
 
 date_time.day   = time_correction(data[0]);
 date_time.month = time_correction(data[1]);
 date_time.year  = time_correction(data[2]);
}

static unsigned char time_correction(unsigned char temp)
{
 if (temp> 9)
  temp-= 6;
 if (temp> 19)
  temp-= 6;
 if (temp> 29)
  temp-= 6;
 if (temp> 39)
  temp-= 6;
 if (temp> 49)
  temp-= 6;
 if (temp> 59)
  temp-= 6;
 if (temp> 69)
  temp-= 6;
 if (temp> 79)
  temp-= 6;
 if (temp> 89)
  temp-= 6;
 if (temp> 99)
  temp-= 6;
  
 return temp;
}

void set_time(void)
{
 uint8_t data[3];
 data[0] = adjust_time(date_time.second);
 data[1] = adjust_time(date_time.minute);
 data[2] = adjust_time(date_time.hour);

 twic_address(ds1307, second_register);
 twic_write(ds1307, data, 3);
}

void set_date(void)
{
 uint8_t data[3];
 data[0] = adjust_time(date_time.day);
 data[1] = adjust_time(date_time.month);
 data[2] = adjust_time(date_time.year);

 twic_address(ds1307, date_register);
 twic_write(ds1307, data, 3);
}

static unsigned char adjust_time(unsigned char temp)
{
  if (temp> 99)
  temp+= 6;
 if (temp> 89)
  temp+= 6;
 if (temp> 79)
  temp+= 6;
 if (temp> 69)
  temp+= 6;
 if (temp> 59)
  temp+= 6;
 if (temp> 49)
  temp+= 6;
 if (temp> 39)
  temp+= 6;
 if (temp> 29)
  temp+= 6;
 if (temp> 19)
  temp+= 6;
 if (temp> 9)
  temp+= 6;
  
 return temp;
}

