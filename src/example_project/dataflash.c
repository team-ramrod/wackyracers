#include "dataflash.h"


static char flash_msg[max_str_length];

__flash static char text[][10]= 
{
 "\rerasing\r", //0
 "\rerased\r", //1 
 ",,", //2
 ",", //3
 ",,,", // 4
};


// Dependent on SPIE (spie.c)

uint8_t df;

//initialise the data flash recording mode and set to capture mode
void dataflash_init(void)
{
 //disable all chips
 dataflash_cs_hi;
 
 dataflash.delay= 10;
 dataflash_reset_lo;
 while (dataflash.delay);
 dataflash_reset_hi;
 dataflash.buffer= 0;
 
 dataflash.flags.recording= bit_false;
 dataflash.flags.startup= bit_true;
 
 df = attach_interrupt(dataflash_interrupt);
}

void dataflash_write(char *ptr) //feed string into data flash
{
 strcpy(flash_msg, ptr);
 dataflash.flags.write= bit_true;
 interrupts[df].flag = bit_true;
}

void dataflash_interrupt(void)
{
 unsigned char i;
 
 if (dataflash_busy()== false)
 {
  if (dataflash.flags.erase)   
   erasing();                   
  
  else if (dataflash.flags.recall_page)
   recalling();                        
  
  else if (dataflash.flags.write)
  {
   for (i= 0; i< strlen(flash_msg); i++)
    write_to_flash(flash_msg[i]);
   dataflash.flags.write= bit_false;
   interrupts[df].flag = bit_false;
  }
 }
 
 //disable all chips
 dataflash_cs_hi;
}

static char dataflash_busy(void)
{
 long i= 0;
 
 WDR();
 if ((dataflash_status() & 0x80) && (i++< 200000))
  return false;
 else
  return true;
}

static unsigned char dataflash_status(void)
{
 unsigned char result;
 
 dataflash_cs_lo; //enable DataFlash
 write_spie(status_register_read); //send status register read op-code
 result= read_spie(); //dummy write to get result
 dataflash_cs_hi; //CS must be high during all transfers including complete transfer of opcode address and any data, then should be deactivated
 
 return result; //return the read status register value
}

static void erasing(void)
{
 static unsigned int block_counter;
 
 if (block_counter< block_length)
 {
  dataflash_cs_lo; // enable DataFlash
  write_spie(block_erase);
  write_spie((char)(block_counter>>3));
  write_spie((char)(block_counter<<5));
  write_spie(0x00);
  dataflash_cs_hi; // disable DataFlash
  block_counter++;  
 }
 else
 {
  block_counter= 0;
  dataflash.buffer= 0;
  dataflash.page= 0;
  ee_write(ee_dataflash_page, dataflash.page, 2, false);
  dataflash.flags.erase= bit_false;
  dataflash.flags.startup= bit_true;
  interrupts[df].flag = bit_false;
 }
}

static void write_to_flash(unsigned char flash_data)
{
 static unsigned char active_buffer;
 
 while (dataflash_busy()); // wait until flash is not busy
 
 dataflash_cs_lo; // enable DataFlash
 dataflash_wp_hi;
 
 if (active_buffer== 1)
  write_spie(buffer_1_write);
 else
  write_spie(buffer_2_write);
 write_spie(0x00);  // don't cares
 write_spie((char)(dataflash.buffer>>8)); // don't cares plus first two bits of buffer address
 write_spie((char) dataflash.buffer); // buffer address (max. 2^8 = 256 pages)
 write_spie(flash_data); // write data into spie Data Register
 
 dataflash_cs_hi; // disable DataFlash
 
 if (++dataflash.buffer> page_length) // if buffer full write buffer into memory page
 {
  dataflash.buffer= 0;
  
  if (dataflash.page< dataflash.gprs_total_pages)
  {
   dataflash_cs_lo; // enable DataFlash
   if (active_buffer== 1)
   {
    write_spie(b1_to_mm_page_prog_with_erase);// write data from buffer 1 to page
    active_buffer= 2;
   }
   else
   {
    write_spie(b2_to_mm_page_prog_with_erase);// write data from buffer 2 to page
    active_buffer= 1;
   }
   write_spie((char)(dataflash.page>>6));
   write_spie((char)(dataflash.page<<2));
   write_spie(0x00); // don't cares
   
   dataflash_cs_hi; // disable DataFlash
   dataflash.page++;
  }
  else
   dataflash.flags.recording= bit_false;
   
  ee_write(ee_dataflash_page, dataflash.page, 2, false);
 }
}

static void recalling(void)
{
 static unsigned char active_buffer; 
 static unsigned int page;
 char local_str[21];
 
 if (page< dataflash.recall_page)
  page= dataflash.recall_page;
 if (active_buffer== 0)
  active_buffer= 1;
 
 if (page< dataflash.recall_page + dataflash.pages)
 {
  next_page_to_next_buffer(active_buffer, page);
  active_buffer_to_port(active_buffer);
  
  if (active_buffer== 1) // if buffer1 is the active buffer
   active_buffer++; // set buffer2 as active buffer
  else
   active_buffer--; // set buffer1 as active buffer
  
  page++;
 }
 else
 {
  page= 0;
  active_buffer= 1;
  dataflash.flags.recall_page= bit_false;
  interrupts[df].flag = bit_false;
 }
}

void next_page_to_next_buffer(unsigned char active_buffer, unsigned int page_counter)
{
 while (dataflash_busy()); // wait until flash is not busy
 
 dataflash_cs_lo; // enable DataFlash
 
 if (active_buffer== 1) // if buffer1 is the active buffer
  write_spie(mm_page_to_b1_xfer); // transfer next page to buffer 1
 else
  write_spie(mm_page_to_b2_xfer); // transfer next page to buffer 2
 
 write_spie((char)(page_counter >> 6));
 write_spie((char)(page_counter << 2));
 write_spie(0x00);

 dataflash_cs_hi; // disable DataFlash and start transaction     
}

static void active_buffer_to_port(unsigned char active_buffer)
{
 static char byte;
 unsigned int buffer_counter= page_length;
 
 while (dataflash_busy()); // wait until flash is not busy
 
 dataflash_cs_lo; // enable DataFlash
 
 if (active_buffer== 1) // if buffer1 is the active buffer
  write_spie(buffer_1_read); // read from buffer 1
 else
  write_spie(buffer_2_read); // read from buffer 2
 
 write_spie(0x00); // write don't care byte
 write_spie(0x00); // write don't care byte
 write_spie(0x00); // start at buffer address 0
 write_spie(0x00); // write don't care byte
 
 do
 {
  byte= read_spie();
 } while (buffer_counter--);
 
 dataflash_cs_hi; // disable DataFlash
}

void save_to_df(void)
{
 char str[max_str_length], tmp_str[21];
 //save to dataflash
 if (dataflash.flags.recording)
 {
  sprintf(str, "\r%02d-%02d-%d,%02d:%02d:%02d.%02d",
   date_time.day, date_time.month, date_time.year+2000, date_time.hour, date_time.minute, date_time.second, 5*date_time.tenth_second);
  
  
  
  if (dataflash.parameters.compass)
   sprintf(tmp_str, ",%d,%d,%d", parameters.compass_x, parameters.compass_y, parameters.compass_z);
  else
   cstrcpy(tmp_str, text[4]);
  strcat(str, tmp_str);
  
  if (dataflash.parameters.gyro)
   sprintf(tmp_str, ",%1.2f,%1.2f", parameters.gyro_x, parameters.gyro_y);
  else
   cstrcpy(tmp_str, text[2]);
  strcat(str, tmp_str);
  
  if (dataflash.parameters.acceleration)
   sprintf(tmp_str, ",%1.2f,%1.2f,%1.2f", parameters.acceleration_x, parameters.acceleration_y, parameters.acceleration_z);
  else
   cstrcpy(tmp_str, text[4]);
  strcat(str, tmp_str);
  
  if (dataflash.parameters.battery_voltage)
   sprintf(tmp_str, ",%1.2f", parameters.battery_voltage);
  else
   cstrcpy(tmp_str, text[3]);
  strcat(str, tmp_str);
  
  if (dataflash.parameters.generator_voltage)
   sprintf(tmp_str, ",%1.2f,%1.2f", parameters.generator_voltage_1, parameters.generator_voltage_1);
  else
   cstrcpy(tmp_str, text[2]);
  strcat(str, tmp_str);
  
  if (dataflash.parameters.load_current)
   sprintf(tmp_str, ",%1.2f,%1.2f", parameters.load_current_1, parameters.load_current_2);
  else
   cstrcpy(tmp_str, text[2]);
  strcat(str, tmp_str);
  
  if (dataflash.parameters.water_pressure)
   sprintf(tmp_str, ",%1.2f", parameters.water_pressure);
  else
   cstrcpy(tmp_str, text[3]);
  strcat(str, tmp_str);
  
  if (dataflash.parameters.hydraulic_pressure)
   sprintf(tmp_str, ",%1.2f,%1.2f", parameters.hydraulic_pressure_1, parameters.hydraulic_pressure_2);
  else
   cstrcpy(tmp_str, text[2]);
  strcat(str, tmp_str);
  
  if (dataflash.parameters.arm_angle)
   sprintf(tmp_str, ",%1.2f", parameters.arm_angle);
  else
   cstrcpy(tmp_str, text[3]);
  strcat(str, tmp_str);
    
  if (dataflash.parameters.relay)
   sprintf(tmp_str, ",%d,%d", control.close_1, control.close_2);
  else
   cstrcpy(tmp_str, text[2]);
  strcat(str, tmp_str);
  
  if (gprs.gps.update && dataflash.parameters.gps)
  {
   sprintf(str, "\r%s", gprs.gps.msg);
   gprs.gps.update= bit_false;
  }
  dataflash_write(str);
 }
}






