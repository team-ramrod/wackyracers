#include "ee.h"

static struct
{
 unsigned char index;
 unsigned char data[buffer_depth];
 unsigned int location[buffer_depth];
} buffer;

void ee_init(void)
{
 if (ee_read(ee_null))
 {
  ee_write(ee_dataflash_page, 0, 2, false);
  ee_write(ee_dataflash_gprs_pages_per_email, 100, 2, false);
  ee_write(ee_dataflash_gprs_no_of_emails, 1, 1, false);
  ee_write(ee_dataflash_gprs_hours, 1, 1, false);
  ee_write(ee_dataflash_parameters_battery_voltage, 1, 1, false);
  ee_write(ee_dataflash_parameters_generator_voltage, 1, 1, false);
  ee_write(ee_dataflash_parameters_load_current, 1, 1, false);
  ee_write(ee_dataflash_parameters_acceleration, 1, 1, false);
  ee_write(ee_dataflash_parameters_water_pressure, 1, 1, false);
  ee_write(ee_dataflash_parameters_hydraulic_pressure, 1, 1, false);
  ee_write(ee_dataflash_parameters_arm_angle, 1, 1, false);
  ee_write(ee_dataflash_parameters_relay, 1, 1, false);
  ee_write(ee_dataflash_parameters_gps, 1, 1, false);
  
  ee_write(ee_energy_joules_byte3, 0, 2, false);
  ee_write(ee_energy_joules_byte1, 0, 2, false);
  
  ee_write(ee_control_latching_relay_close_1, 0, 1, false);
  ee_write(ee_control_latching_relay_close_2, 0, 1, false);
  
  ee_write(ee_null, 0, 1, false);
 }
 
 dataflash.page= ((unsigned int) ee_read(ee_dataflash_page-1) << 8) + ee_read(ee_dataflash_page);
 dataflash.gprs_pages_per_email= ((unsigned int) ee_read(ee_dataflash_gprs_pages_per_email-1) << 8) + ee_read(ee_dataflash_gprs_pages_per_email);
 dataflash.gprs_no_of_emails= ee_read(ee_dataflash_gprs_no_of_emails);
 dataflash.gprs_total_pages= (unsigned int) dataflash.gprs_pages_per_email * dataflash.gprs_no_of_emails;
 dataflash.gprs_hours= ee_read(ee_dataflash_gprs_hours);
 dataflash.parameters.battery_voltage= ee_read(ee_dataflash_parameters_battery_voltage);
 dataflash.parameters.generator_voltage= ee_read(ee_dataflash_parameters_generator_voltage);
 dataflash.parameters.load_current= ee_read(ee_dataflash_parameters_load_current);
 dataflash.parameters.acceleration= ee_read(ee_dataflash_parameters_acceleration);
 dataflash.parameters.water_pressure= ee_read(ee_dataflash_parameters_water_pressure);
 dataflash.parameters.hydraulic_pressure= ee_read(ee_dataflash_parameters_hydraulic_pressure);
 dataflash.parameters.arm_angle= ee_read(ee_dataflash_parameters_arm_angle);
 dataflash.parameters.relay= ee_read(ee_dataflash_parameters_relay);
 dataflash.parameters.gps= ee_read(ee_dataflash_parameters_gps);
 
 energy.joules= ((long) ee_read(ee_energy_joules_byte4) << 8) + ee_read(ee_energy_joules_byte3);
 energy.joules= (long) energy.joules * 65536;
 energy.joules+= ((long) ee_read(ee_energy_joules_byte2) << 8) + ee_read(ee_energy_joules_byte1);
 if (energy.joules< 0)
  energy.joules= 0;
 
 energy.joules= 0;
 energy.peak_kw= 0;
 energy.sample_joules= 0;
 energy.samples= 0;
 
 control.close_1= ee_read(ee_control_latching_relay_close_1);
 control.close_2= ee_read(ee_control_latching_relay_close_2);
}

unsigned char ee_read(unsigned int location)
{
 unsigned char result;
 while (ee_busy); // Wait until any earlier write is done.
 EEPROM_READ(location, result);
 while (ee_busy);
 return result; // Return byte
}

static void load_to_ee(unsigned int location, unsigned char data)
{
 while (ee_busy);
 EEPROM_WRITE(location, data); 
 while (ee_busy);
}

char ee_write(unsigned int location, unsigned int data, unsigned char bytes, unsigned char real_time)
{
 if (real_time== false)
 {
  while (ee_busy); //wait until ready
  load_to_ee(location, data & 0x00ff);
  
  if (bytes== 2)
  {
   while (ee_busy); //wait until ready
   load_to_ee(location-1, data >> 8);
  }
 }
 else
 {
  //load data and address into array
  buffer.index= (buffer.index + 1) % buffer_depth;
  buffer.data[buffer.index]= data & 0x00ff;
  buffer.location[buffer.index]= location;
  
  //load second data and address if required
  if (bytes== 2)
  {
   buffer.index= (buffer.index + 1) % buffer_depth;
   buffer.data[buffer.index]= data >> 8;
   buffer.location[buffer.index]= location - 1;
  }
 }
 
 return true;
}

void maintain_ee(void)
{
 unsigned char i;
 
 if (ee_busy== false)
  for (i= 0; i< buffer_depth; i++)
   if (buffer.location[i]) //check for data to be written to eeprom
   {
    load_to_ee(buffer.location[i], buffer.data[i]);
    buffer.location[i]= 0;
    i= buffer_depth+2;
   }
}

