#include "gprs.h"


// GPRS wait codes
enum 
{
  wt_none= 0,
  wt_ok,
  wt_220,
  wt_250,
  wt_354,
  wt_221,
  wt_ok_or_error_321,
  wt_arrow,
  wt_connect,
  wt_plus_ok,
  wt_dot,
  wt_cmgs,
  wt_ok_or_no_carrier,
} wait_for;


void gprs_init(void)
{
 gprs_ptr= gprs_power_down;
}

void gprs_idle(void)
{
 wait_for= wt_none;
 
 gprs_pwr_hi;
 
 gprs.flags.pwr_on= bit_false;
 gprs.flags.connect= bit_false;
 gprs.flags.idle= bit_true;
 
 gprs_index.power_down= 0;
 gprs_index.gsm_gps= 0;
 gprs_index.gsm_send= 0;
 gprs_index.gsm_inbox= 0;
 gprs_index.email_send= 0;
 
 if (dataflash.flags.startup)
 {
  if (dataflash.parameters.gps)
   gprs_ptr= gprs_gsm_gps;
  else
   dataflash.flags.recording= bit_true;
 }
}

void gprs_power_down(void)
{
 switch (gprs_index.power_down++)
 {
  case 10 :
   if (gprs.flags.pwr_on== bit_false)
    gprs_index.power_down= 98;
   else
    gprs_on_lo;
   break;
  
  case 20 :
   gprs_on_hi;
   break;
  
  case 45 :
   gprs_on_lo;
   gprs_dtr_lo;
   gprs_rts_lo;
   gprs_pwr_hi;
   break;
  
  case 100 :
   gprs_ptr= gprs_idle;
   break;
  
  default :
   if (gprs_index.power_down> 100)
   {
    gprs_index.power_down= 0;
    gprs_ptr= gprs_power_down;
   }
   break;
 }
}

void gprs_gsm_gps(void)
{
 static unsigned char i;
 
 check_wait_states(&gprs_index.gsm_gps);
 gprs.flags.connect= bit_false;
 
 gprs_pwr_lo;
 
 switch (gprs_index.gsm_gps++)
 {
  case 1 :
   if (gprs.flags.pwr_on== bit_true)
    gprs_index.gsm_gps= 40;
   else
   {
    gprs_on_lo;
    gprs_reset_lo;
    gprs_dtr_lo;
   }
   break;
  
  case 10 :
   gprs_reset_hi;
   break;
  
  case 12 :
   gprs_reset_lo;
   break;
  
  case 15 :
   gprs_on_hi;
   break;
  
  case 30 :
   gprs_on_lo;
   break;
  
  case 45 :
   gprs_dtr_lo;
   gprs_rts_lo;
   wait_for= wt_none;
   gprs.flags.pwr_on= bit_true;
   break;
  
  case 50 :
   //at command
   cstrcpy(gprs.str, gprs_text[2]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
  
  case 55 :
   //at command
   cstrcpy(gprs.str, gprs_text[2]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
  
  case 60 :
   //turn echo off
   cstrcpy(gprs.str, gprs_text[0]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
   
  case 65 :
   //turn on gps receiver
   cstrcpy(gprs.str, gprs_text[27]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
  
  case 70 :
   //set gps to cold start
   cstrcpy(gprs.str, gprs_text[28]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
 
  case 75 :
   //set to gps full power mode
   cstrcpy(gprs.str, gprs_text[47]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
   
  case 200 :
   if (dataflash.parameters.gps && (dataflash.flags.startup || dataflash.flags.recording))
   {
    //loop back 1 second
    gprs_index.gsm_gps= 190;
    cstrcpy(gprs.str, gprs_text[29]);
    usartc1_write(gprs.str);
    wait_for= wt_ok;
   }
   else
   {
    //turn off gps receiver
    cstrcpy(gprs.str, gprs_text[30]);
    usartc1_write(gprs.str);
    wait_for= wt_ok;
    gprs_index.gsm_gps++;
   }
   break;
  
  case 210 :
   gprs_index.power_down= 0;
   gprs_ptr= gprs_power_down;
   break;
  
  default :
   if (gprs_index.gsm_gps> 220)
    gprs_index.gsm_gps= 0;
   break;
 }
}

void gprs_email_send(void)
{
 static unsigned char email_packets;
 static unsigned int counter;
 
 check_wait_states(&gprs_index.email_send);
 gprs.flags.connect= bit_true;
 
 gprs_pwr_lo;
 
 switch (gprs_index.email_send++)
 {
  case 1 :
   if (gprs.flags.pwr_on== bit_true)
	gprs_index.email_send= 48;
   else
   {
    gprs_on_lo;
    gprs_reset_lo;
    gprs_dtr_lo;
   }
   email_packets= 0;
   break;
  
  case 10 :
   gprs_reset_hi;
   break;
  
  case 12 :
   gprs_reset_lo;
   break;
  
  case 15 :
   gprs_on_hi;
   break;
  
  case 30 :
   gprs_on_lo;
   break;
  
  case 45 :
   gprs_dtr_lo;
   gprs_rts_lo;
   wait_for= wt_none;
   gprs.flags.pwr_on= bit_true;
   break;
  
  case 50 :
   //at command
   cstrcpy(gprs.str, gprs_text[2]);
   usartc1_write(gprs.str);
   break;
  
  case 55 :
   //at command
   cstrcpy(gprs.str, gprs_text[2]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
  
  case 60 :
   //turn echo off
   cstrcpy(gprs.str, gprs_text[0]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
  
  case 65 :
   //mobile full functionality with power saving disabled (factory default)
   cstrcpy(gprs.str, gprs_text[3]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
   
  case 70 :
   //internet configuration
   cstrcpy(gprs.str, gprs_text[34]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 80 :
   //scfg settings
   cstrcpy(gprs.str, gprs_text[35]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 90 :
   //sgact settings
   cstrcpy(gprs.str, gprs_text[36]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
 
  case 100 :
   //sd settings
   //cstrcpy(gprs.str, gprs_text[37]);
   //set_quotes(gprs.str);
   //usartc1_write(gprs.str);
   //wait_for= wt_ok;
   //gprs_index.email_send++;
   break;
  
  case 110 :
   //data send time-out= off
   //cstrcpy(gprs.str, gprs_text[8]);
   //usartc1_write(gprs.str);
   //wait_for= wt_ok;
   //gprs_index.email_send++;
   break;
  
  case 120 :
   //socket inactivity time-out= 900s
   //cstrcpy(gprs.str, gprs_text[9]);
   //usartc1_write(gprs.str);
   //wait_for= wt_ok;
   //gprs_index.email_send++;
   break;
  
  case 130 :
   //socket connection time-out= 600ms
   //cstrcpy(gprs.str, gprs_text[10]);
   //usartc1_write(gprs.str);
   //wait_for= wt_ok;
   //gprs_index.email_send++;
   break;
  
  case 140 :
   //save socket settings
   //cstrcpy(gprs.str, gprs_text[11]);
   //usartc1_write(gprs.str);
   //wait_for= wt_ok;
   //gprs_index.email_send++;
   break;
  
  case 150 :
   //make gprs connection
   //cstrcpy(gprs.str, gprs_text[12]);
   //usartc1_write(gprs.str);
   //wait_for= wt_ok;
   //gprs_index.email_send++;
   break;
   
  case 160 :
   //dial socket
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[37]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_220;
   gprs_index.email_send++;
   gprs_rts_lo;
   gprs.flags.connect= bit_true;
   break;
  
  case 161 :
   gprs_rts_lo;
   break;
  
  case 180 :
   //helo
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[38]);
   usartc1_write(gprs.str);
   wait_for= wt_250;
   gprs_index.email_send++;
   gprs.flags.connect= bit_false;
   break;
  
  case 181 :
   gprs_rts_lo;
   break;
  
  case 200 :
   //mail from
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[39]);
   usartc1_write(gprs.str);
   wait_for= wt_250;
   gprs_index.email_send++;
   break;
  
  case 201 :
   gprs_rts_lo;
   try_alternative_email_address= bit_false;
   break;
  
  case 220 :
   //mail to
   gprs_rts_hi;
   if (try_alternative_email_address)
    cstrcpy(gprs.str, gprs_text[42]);
   else
    cstrcpy(gprs.str, gprs_text[40]);
   usartc1_write(gprs.str);
   wait_for= wt_250;
   gprs_index.email_send++;
   break;
  
  case 221 :
   gprs_rts_lo;
   break;
  
  case 240 :
   //load data
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[17]);
   usartc1_write(gprs.str);
   wait_for= wt_354;
   gprs_index.email_send++;
   break;
  
  case 241 :
   gprs_rts_lo;
   break;
  
  case 260 :
   //mail from address
   cstrcpy(gprs.str, gprs_text[18]);
   usartc1_write(gprs.str);
   break;
  
  case 270 :
   //mail to address
   cstrcpy(gprs.str, gprs_text[19]);
   usartc1_write(gprs.str);
   break;
  
  case 280 :
   //mail subject
   cstrcpy(gprs.str, gprs_text[20]);
   usartc1_write(gprs.str);
   break;
  
  case 290 :
   cstrcpy(gprs.str, gprs_text[78]);
   usartc1_write(gprs.str);
   wait_for= wt_none;
   break;
  
  case 291 :
   if (control.close_1)
    cstrcpy(gprs.str, gprs_text[79]);
   else
    cstrcpy(gprs.str, gprs_text[86]);
   usartc1_write(gprs.str);
   break;
  
  case 292 :
   if (control.close_2)
    cstrcpy(gprs.str, gprs_text[87]);
   else
    cstrcpy(gprs.str, gprs_text[88]);
   usartc1_write(gprs.str);
   break;
   
  case 293 :
   cstrcpy(gprs.str, gprs_text[82]);
   sprintf(gprs.str,"%s%ldkJ\r", gprs.str, energy.joules/1000);
   usartc1_write(gprs.str);
   break;
  
  case 294 :
   cstrcpy(gprs.str, gprs_text[83]);
   sprintf(gprs.str,"%s%fkW\r", gprs.str, energy.peak_kw);
   usartc1_write(gprs.str);
   break;
  
  case 295 :
   cstrcpy(gprs.str, gprs_text[84]);
   sprintf(gprs.str,"%s%ldJ (%ld)\r", gprs.str, energy.sample_joules, energy.samples);
   usartc1_write(gprs.str);
   break;
  
  case 296 :
   cstrcpy(gprs.str, gprs_text[44]);
   sprintf(gprs.str,"%s%fhr\r", gprs.str, (float) dataflash.gprs_hours/10);
   usartc1_write(gprs.str);
   break;
  
  case 297 :
   cstrcpy(gprs.str, gprs_text[80]);
   sprintf(gprs.str,"%s%d\r", gprs.str, dataflash.gprs_total_pages);
   usartc1_write(gprs.str);
   break;
  
  case 298 :
   cstrcpy(gprs.str, gprs_text[45]);
   usartc1_write(gprs.str);
   break;
  
  case 299 :
   if (dataflash.parameters.acceleration)
    cstrcpy(gprs.str, gprs_text[49]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 300 :
   if (dataflash.parameters.battery_voltage)
    cstrcpy(gprs.str, gprs_text[50]);
   else
    cstrcpy(gprs.str, gprs_text[26]);
   usartc1_write(gprs.str);
   break;
  
  case 301 :
   if (dataflash.parameters.generator_voltage)
    cstrcpy(gprs.str, gprs_text[51]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 302 :
   if (dataflash.parameters.load_current)
    cstrcpy(gprs.str, gprs_text[52]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 303 :
   if (dataflash.parameters.water_pressure)
    cstrcpy(gprs.str, gprs_text[53]);
   else
    cstrcpy(gprs.str, gprs_text[26]);
   usartc1_write(gprs.str);
   break;
  
  case 304 :
   if (dataflash.parameters.hydraulic_pressure)
    cstrcpy(gprs.str, gprs_text[54]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 305 :
   if (dataflash.parameters.arm_angle)
    cstrcpy(gprs.str, gprs_text[81]);
   else
    cstrcpy(gprs.str, gprs_text[26]);
   usartc1_write(gprs.str);
   break;
   break;
  
  case 307 :
   if (dataflash.parameters.relay)
    cstrcpy(gprs.str, gprs_text[103]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 308 :
   cstrcpy(gprs.str, gprs_text[21]);
   usartc1_write(gprs.str);
   break;
  
  case 309 :
   dataflash.flags.recall_page= bit_true;
   interrupts[df].flag = bit_true;
   dataflash.recall_page= dataflash.gprs_pages_per_email * email_packets;
   dataflash.pages= dataflash.gprs_pages_per_email;
   break;
  
  case 320 :
   if (dataflash.flags.recall_page)
    gprs_index.email_send--;
   break;
  
  case 330 :
   //end of email text
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[22]);
   usartc1_write(gprs.str);
   wait_for= wt_250;
   gprs_index.email_send++;
   counter= 0;
   break;
  
  case 331 :
   gprs_rts_lo;
   if (counter++> gprs_timeout_1min)
    wait_for= wt_none;
   break;
  
  case 340 :
   //quit
   gprs_rts_hi;
   if (++email_packets>= dataflash.gprs_no_of_emails)
   {
    cstrcpy(gprs.str, gprs_text[23]);
    usartc1_write(gprs.str);
    wait_for= wt_221;
    gprs_index.email_send++;
   }
   else
    gprs_index.email_send= 190;
   counter= 0;
   break;
  
  case 341 :
   gprs_rts_lo;
   if (counter++> gprs_timeout_1min)
    wait_for= wt_none;
   break;
  
  case 350 :
   //exit
   gprs_rts_lo;
   cstrcpy(gprs.str, gprs_text[24]);
   usartc1_write(gprs.str);
   wait_for= wt_ok_or_no_carrier;
   gprs_index.email_send++;
   counter= 0;
   break;
  
  case 351 :
   gprs_rts_lo;
   if (counter++> gprs_timeout_1min)
    wait_for= wt_none;
   break;
  
  case 360 :
   //exit
   gprs_rts_lo;
   cstrcpy(gprs.str, gprs_text[41]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   counter= 0;
   break;
  
  case 361 :
   gprs_rts_lo;
   if (counter++> gprs_timeout_1min)
    wait_for= wt_none;
   break;
  
  case 370 :
   gprs_ptr= gprs_gsm_inbox;
   energy.peak_kw= 0;
   energy.sample_joules= 0;
   energy.samples= 0;
   
   dataflash.flags.erase= bit_true;
   interrupts[df].flag = bit_true;
   break;
  
  default :
   if (gprs_index.email_send> 370)
    gprs_ptr= gprs_power_down;
   break;
 }
}

/*void gprs_email_send(void)
{
 static unsigned char email_packets;
 static unsigned int counter;
 
 check_wait_states(&gprs_index.email_send);
 gprs.flags.connect= bit_true;
 
 gprs_pwr_lo;
 
 switch (gprs_index.email_send++)
 {
  case 1 :
   if (gprs.flags.pwr_on== bit_true)
	gprs_index.email_send= 48;
   else
   {
    gprs_on_lo;
    gprs_reset_lo;
    gprs_dtr_lo;
   }
   email_packets= 0;
   break;
  
  case 10 :
   gprs_reset_hi;
   break;
  
  case 12 :
   gprs_reset_lo;
   break;
  
  case 15 :
   gprs_on_hi;
   break;
  
  case 30 :
   gprs_on_lo;
   break;
  
  case 45 :
   gprs_dtr_lo;
   gprs_rts_lo;
   wait_for= wt_none;
   gprs.flags.pwr_on= bit_true;
   break;
  
  case 50 :
   //at command
   cstrcpy(gprs.str, gprs_text[2]);
   usartc1_write(gprs.str);
   break;
  
  case 55 :
   //at command
   cstrcpy(gprs.str, gprs_text[2]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
  
  case 60 :
   //turn echo off
   cstrcpy(gprs.str, gprs_text[0]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_gps++;
   break;
  
  case 65 :
   //mobile full functionality with power saving disabled (factory default)
   cstrcpy(gprs.str, gprs_text[3]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
   
  case 70 :
   //internet configuration
   cstrcpy(gprs.str, gprs_text[4]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 80 :
   //userid= guest
   cstrcpy(gprs.str, gprs_text[5]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 90 :
   //password= guest
   cstrcpy(gprs.str, gprs_text[6]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
 
  case 100 :
   //packet size= 300
   cstrcpy(gprs.str, gprs_text[7]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 110 :
   //data send time-out= off
   cstrcpy(gprs.str, gprs_text[8]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 120 :
   //socket inactivity time-out= 900s
   cstrcpy(gprs.str, gprs_text[9]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 130 :
   //socket connection time-out= 600ms
   cstrcpy(gprs.str, gprs_text[10]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 140 :
   //save socket settings
   cstrcpy(gprs.str, gprs_text[11]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
  
  case 150 :
   //make gprs connection
   cstrcpy(gprs.str, gprs_text[12]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.email_send++;
   break;
   
  case 160 :
   //dial socket
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[13]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_220;
   gprs_index.email_send++;
   gprs_rts_lo;
   gprs.flags.connect= bit_true;
   break;
  
  case 161 :
   gprs_rts_lo;
   break;
  
  case 180 :
   //helo
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[14]);
   usartc1_write(gprs.str);
   wait_for= wt_250;
   gprs_index.email_send++;
   gprs.flags.connect= bit_false;
   break;
  
  case 181 :
   gprs_rts_lo;
   break;
  
  case 200 :
   //mail from
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[15]);
   usartc1_write(gprs.str);
   wait_for= wt_250;
   gprs_index.email_send++;
   break;
  
  case 201 :
   gprs_rts_lo;
   break;
  
  case 220 :
   //mail to
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[16]);
   usartc1_write(gprs.str);
   wait_for= wt_250;
   gprs_index.email_send++;
   break;
  
  case 221 :
   gprs_rts_lo;
   break;
  
  case 240 :
   //load data
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[17]);
   usartc1_write(gprs.str);
   wait_for= wt_354;
   gprs_index.email_send++;
   break;
  
  case 241 :
   gprs_rts_lo;
   break;
  
  case 260 :
   //mail from address
   cstrcpy(gprs.str, gprs_text[18]);
   usartc1_write(gprs.str);
   break;
  
  case 270 :
   //mail to address
   cstrcpy(gprs.str, gprs_text[19]);
   usartc1_write(gprs.str);
   break;
  
  case 280 :
   //mail subject
   cstrcpy(gprs.str, gprs_text[20]);
   usartc1_write(gprs.str);
   break;
  
  case 290 :
   cstrcpy(gprs.str, gprs_text[78]);
   usartc1_write(gprs.str);
   wait_for= wt_none;
   break;
  
  case 291 :
   if (control.latching_relay.close_1)
    cstrcpy(gprs.str, gprs_text[79]);
   else
    cstrcpy(gprs.str, gprs_text[86]);
   usartc1_write(gprs.str);
   break;
  
  case 292 :
   if (control.latching_relay.close_2)
    cstrcpy(gprs.str, gprs_text[87]);
   else
    cstrcpy(gprs.str, gprs_text[88]);
   usartc1_write(gprs.str);
   break;
   
  case 293 :
   cstrcpy(gprs.str, gprs_text[82]);
   sprintf(gprs.str,"%s%ldkJ\r", gprs.str, energy.joules/1000);
   usartc1_write(gprs.str);
   break;
  
  case 294 :
   cstrcpy(gprs.str, gprs_text[83]);
   sprintf(gprs.str,"%s%fkW\r", gprs.str, energy.peak_kw);
   usartc1_write(gprs.str);
   break;
  
  case 295 :
   cstrcpy(gprs.str, gprs_text[84]);
   sprintf(gprs.str,"%s%ldJ (%ld)\r", gprs.str, energy.sample_joules, energy.samples);
   usartc1_write(gprs.str);
   break;
  
  case 296 :
   cstrcpy(gprs.str, gprs_text[44]);
   sprintf(gprs.str,"%s%fhr\r", gprs.str, (float) dataflash.gprs_hours/10);
   usartc1_write(gprs.str);
   break;
  
  case 297 :
   cstrcpy(gprs.str, gprs_text[80]);
   sprintf(gprs.str,"%s%d\r", gprs.str, dataflash.gprs_total_pages);
   usartc1_write(gprs.str);
   break;
  
  case 298 :
   cstrcpy(gprs.str, gprs_text[45]);
   usartc1_write(gprs.str);
   break;
  
  case 299 :
   if (dataflash.parameters.acceleration)
    cstrcpy(gprs.str, gprs_text[49]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 300 :
   if (dataflash.parameters.battery_voltage)
    cstrcpy(gprs.str, gprs_text[50]);
   else
    cstrcpy(gprs.str, gprs_text[26]);
   usartc1_write(gprs.str);
   break;
  
  case 301 :
   if (dataflash.parameters.generator_voltage)
    cstrcpy(gprs.str, gprs_text[51]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 302 :
   if (dataflash.parameters.load_current)
    cstrcpy(gprs.str, gprs_text[52]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 303 :
   if (dataflash.parameters.water_pressure)
    cstrcpy(gprs.str, gprs_text[53]);
   else
    cstrcpy(gprs.str, gprs_text[26]);
   usartc1_write(gprs.str);
   break;
  
  case 304 :
   if (dataflash.parameters.hydraulic_pressure)
    cstrcpy(gprs.str, gprs_text[54]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 305 :
   if (dataflash.parameters.arm_angle)
    cstrcpy(gprs.str, gprs_text[81]);
   else
    cstrcpy(gprs.str, gprs_text[26]);
   usartc1_write(gprs.str);
   break;
  
  case 306 :
   if (dataflash.parameters.hydraulic_pressure_kelvin)
    cstrcpy(gprs.str, gprs_text[65]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 307 :
   if (dataflash.parameters.arm_angle_kelvin)
    cstrcpy(gprs.str, gprs_text[91]);
   else
    cstrcpy(gprs.str, gprs_text[26]);
   usartc1_write(gprs.str);
   break;
  
  case 308 :
   if (dataflash.parameters.relay)
    cstrcpy(gprs.str, gprs_text[103]);
   else
    cstrcpy(gprs.str, gprs_text[31]);
   usartc1_write(gprs.str);
   break;
  
  case 309 :
   cstrcpy(gprs.str, gprs_text[21]);
   usartc1_write(gprs.str);
   break;
  
  case 310 :
   dataflash.flags.recall_page= bit_true;
   interrupts[df].flag = bit_true;
   dataflash.recall_page= dataflash.gprs_pages_per_email * email_packets;
   dataflash.pages= dataflash.gprs_pages_per_email;
   break;
  
  case 320 :
   if (dataflash.flags.recall_page)
    gprs_index.email_send--;
   break;
  
  case 330 :
   //end of email text
   gprs_rts_hi;
   cstrcpy(gprs.str, gprs_text[22]);
   usartc1_write(gprs.str);
   wait_for= wt_250;
   gprs_index.email_send++;
   counter= 0;
   break;
  
  case 331 :
   gprs_rts_lo;
   if (counter++> gprs_timeout_1min)
    wait_for= wt_none;
   break;
  
  case 340 :
   //quit
   gprs_rts_hi;
   if (++email_packets>= dataflash.gprs_no_of_emails)
   {
    cstrcpy(gprs.str, gprs_text[23]);
    usartc1_write(gprs.str);
    wait_for= wt_221;
    gprs_index.email_send++;
   }
   else
    gprs_index.email_send= 190;
   counter= 0;
   break;
  
  case 341 :
   gprs_rts_lo;
   if (counter++> gprs_timeout_1min)
    wait_for= wt_none;
   break;
  
  case 350 :
   //exit
   gprs_rts_lo;
   cstrcpy(gprs.str, gprs_text[24]);
   usartc1_write(gprs.str);
   wait_for= wt_no_carrier;
   gprs_index.email_send++;
   break;
  
  case 351 :
   gprs_rts_lo;
   break;
  
  case 360 :
   gprs_ptr= gprs_gsm_inbox;
   energy.peak_kw= 0;
   energy.sample_joules= 0;
   energy.samples= 0;
   
   dataflash.flags.erase= bit_true;
   interrupts[df].flag = bit_true;
   break;
  
  default :
   if (gprs_index.email_send> 360)
    gprs_ptr= gprs_power_down;
   break;
 }
}*/

void gprs_gsm_inbox(void)
{
 check_wait_states(&gprs_index.gsm_inbox);
 gprs.flags.connect= bit_false;
 
 gprs_pwr_lo;
 
 switch (gprs_index.gsm_inbox++)
 {
  case 10 :
   //service information
   cstrcpy(gprs.str, gprs_text[1]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_inbox++;
   break;
  
  case 15 :
   //message format in text
   cstrcpy(gprs.str, gprs_text[25]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_inbox++;
   break;
  
  case 20 :
   //service centre address
   cstrcpy(gprs.str, gprs_text[32]);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_inbox++;
   break;

  case 245 :
   //recall unread text messages
   cstrcpy(gprs.str, gprs_text[70]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_inbox++;
   break;
  
  case 290 :
   //delete all read texts
   cstrcpy(gprs.str, gprs_text[48]);
   set_quotes(gprs.str);
   usartc1_write(gprs.str);
   wait_for= wt_ok;
   gprs_index.gsm_inbox++;
   break;
  
  default :
   if (gprs_index.gsm_inbox> 300)
    gprs_ptr= gprs_power_down;
   break;
 }
}

void gprs_gsm_send(void)
{
 check_wait_states(&gprs_index.gsm_send);
 gprs.flags.connect= bit_false;
 
 gprs_pwr_lo;
 
 switch (gprs_index.gsm_send++)
 {
  case 10 :
   //text message
   cstrcpy(gprs.str, gprs_text[33]);
   sprintf(gprs.str,"%s%s\r", gprs.str, gprs.gsm.cell_number);
   usartc1_write(gprs.str);
   wait_for= wt_arrow;
   gprs_index.gsm_send++;
   gprs.gsm.msg[0]= ch_null;
   break;
  
  case 20 :
   create_gsm_msg();
   usartc1_write(gprs.gsm.msg);
   wait_for= wt_ok;
   gprs_index.gsm_send++;
   break;
  
  default :
   if (gprs_index.gsm_send> 50)
   {
    gprs_index.gsm_send= 0;
    if (dataflash.parameters.gps)
     gprs_ptr= gprs_gsm_gps;
    else
     gprs_ptr= gprs_power_down;
   }
   break; 
 }
}

static void create_gsm_msg(void)
{
 char tmp_str[21];
 
 if (control.close_1)
  cstrcpy(tmp_str, gprs_text[79]);
 else
  cstrcpy(tmp_str, gprs_text[86]);
 sprintf(gprs.gsm.msg,"%s", tmp_str);
 
 if (control.close_2)
  cstrcpy(tmp_str, gprs_text[87]);
 else
  cstrcpy(tmp_str, gprs_text[88]);
 sprintf(gprs.gsm.msg,"%s%s", gprs.gsm.msg, tmp_str);
 
 cstrcpy(tmp_str, gprs_text[44]);
 sprintf(gprs.gsm.msg,"%s%s%fhr\r", gprs.gsm.msg, tmp_str, (float) dataflash.gprs_hours/10);
 
 cstrcpy(tmp_str, gprs_text[80]);
 sprintf(gprs.gsm.msg,"%s%s%d\r", gprs.gsm.msg, tmp_str, dataflash.gprs_total_pages);
 
 cstrcpy(tmp_str, gprs_text[82]);
 sprintf(gprs.gsm.msg,"%s%s%ldkJ\r", gprs.gsm.msg, tmp_str, energy.joules/1000);
 
 if (dataflash.parameters.battery_voltage)
  sprintf(gprs.gsm.msg,"%sb", gprs.gsm.msg);
 
 if (dataflash.parameters.generator_voltage)
  sprintf(gprs.gsm.msg,"%sv", gprs.gsm.msg);
 
 if (dataflash.parameters.load_current)
  sprintf(gprs.gsm.msg,"%si", gprs.gsm.msg);
 
 if (dataflash.parameters.acceleration)
  sprintf(gprs.gsm.msg,"%sx", gprs.gsm.msg);
 
 if (dataflash.parameters.water_pressure)
  sprintf(gprs.gsm.msg,"%sw", gprs.gsm.msg);
 
 if (dataflash.parameters.hydraulic_pressure)
  sprintf(gprs.gsm.msg,"%sh", gprs.gsm.msg);
 
 if (dataflash.parameters.arm_angle)
  sprintf(gprs.gsm.msg,"%sa", gprs.gsm.msg);
 
 if (dataflash.parameters.relay)
  sprintf(gprs.gsm.msg,"%sr", gprs.gsm.msg);
 
 if (dataflash.parameters.gps)
  sprintf(gprs.gsm.msg,"%sg", gprs.gsm.msg);
 
 sprintf(gprs.gsm.msg,"%s %c", gprs.gsm.msg, ch_ctrl_z);
}

static void check_wait_states(int *i)
{
 static unsigned int timeout, timeout_length;
 static unsigned char attempts, error_counts;
 unsigned int gprs_easy_code;
 
 gprs.flags.idle= bit_false;
 
 if (gprs.flags.connect)
  timeout_length= gprs_timeout_3min;
 else
  timeout_length= 100;
 
 switch (wait_for)
 {
  case wt_none :
   timeout= 0;
   break;
  
  case wt_dot :
   if (gprs.msg[0]!= '.')
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
   
  case wt_arrow :
   if (gprs.msg[0]!= '>')
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_ok_or_error_321 :
   if (cstrncmp(gprs_text[55], gprs.msg, 15) && cstrncmp(gprs_text[56], gprs.msg, cstrlen(gprs_text[56])))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_ok :
   if (cstrncmp(gprs_text[56], gprs.msg, cstrlen(gprs_text[56])))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
	error_counts= 0;
   }
   break;
  
  case wt_220 :
   if (cstrncmp(gprs_text[57], gprs.msg, 3))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_250 :
   if (cstrncmp(gprs_text[58], gprs.msg, 3))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_354 :
   if (cstrncmp(gprs_text[59], gprs.msg, 3))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_221 :
   if (cstrncmp(gprs_text[60], gprs.msg, cstrlen(gprs_text[60])))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_connect :
   if (cstrncmp(gprs_text[61], gprs.msg, cstrlen(gprs_text[61])))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_plus_ok :
   if (cstrncmp(gprs_text[46], gprs.msg, cstrlen(gprs_text[46])))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_cmgs :
   if (cstrncmp(gprs_text[77], gprs.msg, cstrlen(gprs_text[77])))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
  
  case wt_ok_or_no_carrier :
   if (cstrncmp(gprs_text[93], gprs.msg, 93) && cstrncmp(gprs_text[56], gprs.msg, cstrlen(gprs_text[56])))
    (*i)--;
   else
   {
    wait_for= wt_none;
	gprs.msg[0]= null;
	timeout= 0;
	attempts= 0;
   }
   break;
 }
 
 if (timeout++> timeout_length)
 {
  if (attempts++< 3)
  {
   (*i)= 0;
   wait_for= wt_none;
   gprs.msg[0]= null;
  }
  else
  {
   gprs_ptr= gprs_power_down;
   gprs_index.power_down= 0;
   gprs.flags.pwr_on= bit_false;
   wait_for= wt_none;
   gprs.msg[0]= null;
   timeout= 0;
   attempts= 0;
   error_counts= 0;
   dataflash.flags.startup= bit_false;
   dataflash.flags.recording= bit_false;
   dataflash.flags.erase= bit_true;
   interrupts[df].flag = bit_true;
  }
  timeout= 0;
 }
 
 if ((cstrncmp(gprs_text[62], gprs.msg, cstrlen(gprs_text[62]))== 0) || 
     (cstrncmp(gprs_text[63], gprs.msg, 4)== 0))
 {
  //wait for 500ms for next attempt
  (*i)-= 5;
  gprs.msg[0]= null;
  wait_for= wt_none;
  if (error_counts++> 20)
  {
   gprs_ptr= gprs_power_down;
   gprs_index.power_down= 0;
   gprs.flags.pwr_on= bit_false;
   wait_for= wt_none;
   gprs.msg[0]= null;
   timeout= 0;
   attempts= 0;
   error_counts= 0;
   dataflash.flags.startup= bit_false;
   dataflash.flags.recording= bit_false;
   dataflash.flags.erase= bit_true;
   interrupts[df].flag = bit_true;
  }
 }
 
 //check for gprs easy error codes
 gprs.msg[3]= ch_null;
 gprs_easy_code= atoi(gprs.msg);
 
 if ((gprs_easy_code>= 550) && (gprs_easy_code<= 564))
 {
  //wait for 500ms for next attempt
  (*i)-= 5;
  gprs.msg[0]= null;
  wait_for= wt_none;
  
  //try alternative email address after 10 attempts
  if (error_counts++> 10)
   try_alternative_email_address= bit_true;
  
  if (error_counts++> 20)
  {
   gprs_ptr= gprs_power_down;
   gprs_index.power_down= 0;
   gprs.flags.pwr_on= bit_false;
   wait_for= wt_none;
   gprs.msg[0]= null;
   timeout= 0;
   attempts= 0;
   error_counts= 0;
   dataflash.flags.startup= bit_false;
   dataflash.flags.recording= bit_false;
   dataflash.flags.erase= bit_true;
   interrupts[df].flag = bit_true;
  }
 }
}

static void set_quotes(char *ptr)
{
 while (*ptr!= 0x00)
 {
  if (*ptr== ch_single_quote)
   *ptr= ch_quote;
  ptr++;
 }
}
