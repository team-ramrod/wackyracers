#include "common.h"

//local variables
static char *tx0_ptr;
static char tx0_fo;
static char rx0_msg[max_str_length];
static char tmp_rx0_msg[max_str_length];


__flash static char text[][30]= 
{
 ":rlys", //0
 ":epag", //1
 ":enum", //2
 ":ehrs", //3
 ":para", //4
 "", //5
 "", //6
 "", //7
 "", //8
 "", //9
 ">", //10
 "", //11
 "", //12
 "", //13
 "", //14
 "", //15
 "", //16
 "", //17
 "", //18
 "", //19
 "", //20
 "", //21
 "", //22
 "", //23
 "", //24
 "", //25
 "", //26
 "", //27
 "", //28
 "", //29
 "", //30
 "", //31
 "", //32
 "+CMGL:", //33
 "", //34
 "+6421651405", //35
 "", //36
 "", //37
 "", //38
 "", //39
 "", //40
 "", //41
 "", //42
};

//UART0 initialize
// desired baud rate:9600
// actual baud rate:9615 (0.2%)
// char size: 8 bit
// parity: Disabled
void usartd0_init(void)
{
 USARTD0.DATA = 0x00;
 USARTD0.CTRLA = 0x2A;
 USARTD0.CTRLB = 0x18;
 USARTD0.CTRLC = 0x03;
 USARTD0.BAUDCTRLA = 0xD0;
 USARTD0.BAUDCTRLB = 0x00;
}


void usartd0_rx_isr(void)
{
 char ch;
 static signed char k;
 
 WDR();
 
 //uart has received a character in UDR
 ch= USARTD0.DATA;
 
 if (k< max_str_length-1)
  k++;
 
 switch (ch)
 {
  case '[' :
   k= 0;
   rx0_msg[k]= ch;
   break;
  
  case ']' :
   rx0_msg[k]= ch;
   rx0_msg[k+1]= null;
   check_valid_command();
   
   for (k= 0; k< max_str_length; k++) //clear string
    rx0_msg[k]= null;
   break;
  
  case '>' :
   cstrcpy(gprs.msg, text[10]);
   break;
  
  case '\r' :
   rx0_msg[k]= null;
   
   switch (rx0_msg[0])
   {
    case '$' :
	 rx0_msg[max_str_length-1]= null;
	 strcpy(gprs.gps.msg, rx0_msg);
	 gprs.gps.update= bit_true;
	 dataflash.flags.recording= bit_true;
	 dataflash.flags.startup= bit_false;
	 break;
	
	case '+' :
	 strcpy(tmp_rx0_msg, rx0_msg);
	 break;
	
	case ':' :
	 check_valid_command();
	 break;
   }
   
   strncpy(gprs.msg, rx0_msg, 51);
   for (k= 0; k< max_str_length; k++) //clear string
    rx0_msg[k]= null;
   k= -1;
   break;
  
  case '\n' :
   k= -1;
   break;
   
  default :
   rx0_msg[k]= ch;
   break;
 }
}

static char check_valid_command(void)
{
 unsigned int data;
 char *s, buf[21];
 unsigned char i;
 
 get_cell_number();
 
 //latching relay settings
 if (check_string_command(0))
 {
  gprs_ptr= gprs_gsm_send;
  switch (rx0_msg[5])
  {
   case 'c' :
    control.close_1= bit_true;
	break;
   case 'o' :
    control.close_1= bit_false;
	break;
  }
  switch (rx0_msg[6])
  {
   case 'c' :
    control.close_2= bit_true;
	break;
   case 'o' :
    control.close_2= bit_false;
	break;
  }
  ee_write(ee_control_latching_relay_close_1, control.close_1, 1, true);
  ee_write(ee_control_latching_relay_close_2, control.close_2, 1, true);
  
  return 0;
 }
 
 //pages per email
 if (check_string_command(1))
 {
  gprs_ptr= gprs_gsm_send;
  data= str2int(rx0_msg, 6, 9);
  if ((data< 1001) && (data> 9))
   dataflash.gprs_pages_per_email= data;
  ee_write(ee_dataflash_gprs_pages_per_email, dataflash.gprs_pages_per_email, 2, true);
  dataflash.gprs_total_pages= dataflash.gprs_pages_per_email * dataflash.gprs_no_of_emails;
  
  return 0;
 }
 
 //number of email
 if (check_string_command(2))
 {
  gprs_ptr= gprs_gsm_send;
  data= str2int(rx0_msg, 6, 8);
  if ((data< 11) && (data> 0))
   dataflash.gprs_no_of_emails= data;
  ee_write(ee_dataflash_gprs_no_of_emails, dataflash.gprs_no_of_emails, 1, true);
  dataflash.gprs_total_pages= dataflash.gprs_pages_per_email * dataflash.gprs_no_of_emails;
  
  return 0;
 }
 
 //hours between gprs emails
 if (check_string_command(3))
 {
  gprs_ptr= gprs_gsm_send;
  data= str2int(rx0_msg, 6, 8);
  if ((data< 240) && (data> 0))
   dataflash.gprs_hours= data;
  ee_write(ee_dataflash_gprs_hours, dataflash.gprs_hours, 1, true);
  
  return 0;
 }
 
 //dataflash parameters
 if (check_string_command(4))
 {
  gprs_ptr= gprs_gsm_send;
  
  for (i= 6; i<=strlen(rx0_msg); i++)
   buf[i-6]= tolower(rx0_msg[i]);
  
  s= strrchr (buf, 'b');
  if (s!= null)
   dataflash.parameters.battery_voltage= bit_true;
  else
   dataflash.parameters.battery_voltage= bit_false;
  
  s= strrchr (buf, 'v');
  if (s!= null)
   dataflash.parameters.generator_voltage= bit_true;
  else
   dataflash.parameters.generator_voltage= bit_false;
  
  s= strrchr (buf, 'i');
  if (s!= null)
   dataflash.parameters.load_current= bit_true;
  else
   dataflash.parameters.load_current= bit_false;
  
  s= strrchr (buf, 'x');
  if (s!= null)
   dataflash.parameters.acceleration= bit_true;
  else
   dataflash.parameters.acceleration= bit_false;
  
  s= strrchr (buf, 'w');
  if (s!= null)
   dataflash.parameters.water_pressure= bit_true;
  else
   dataflash.parameters.water_pressure= bit_false;
  
  s= strrchr (buf, 'h');
  if (s!= null)
   dataflash.parameters.hydraulic_pressure= bit_true;
  else
   dataflash.parameters.hydraulic_pressure= bit_false;
  
  s= strrchr (buf, 'a');
  if (s!= null)
   dataflash.parameters.arm_angle= bit_true;
  else
   dataflash.parameters.arm_angle= bit_false;
  
  s= strrchr (buf, 'r');
  if (s!= null)
   dataflash.parameters.relay= bit_true;
  else
   dataflash.parameters.relay= bit_false;
  
  s= strrchr (buf, 'g');
  if (s!= null)
   dataflash.parameters.gps= bit_true;
  else
   dataflash.parameters.gps= bit_false;
  
  ee_write(ee_dataflash_parameters_acceleration, dataflash.parameters.acceleration, 1, true);
  ee_write(ee_dataflash_parameters_battery_voltage, dataflash.parameters.battery_voltage, 1, true);
  ee_write(ee_dataflash_parameters_generator_voltage, dataflash.parameters.generator_voltage, 1, true);
  ee_write(ee_dataflash_parameters_load_current, dataflash.parameters.load_current, 1, true);
  ee_write(ee_dataflash_parameters_water_pressure, dataflash.parameters.water_pressure, 1, true);
  ee_write(ee_dataflash_parameters_hydraulic_pressure, dataflash.parameters.hydraulic_pressure, 1, true);
  ee_write(ee_dataflash_parameters_arm_angle, dataflash.parameters.arm_angle, 1, true);
  ee_write(ee_dataflash_parameters_relay, dataflash.parameters.relay, 1, true);
  ee_write(ee_dataflash_parameters_gps, dataflash.parameters.gps, 1, true);
  
  return 0;
 }
 
 return 0;
}

static char check_string_command(unsigned char i)
{
 if (cstrncmp(text[i], rx0_msg, cstrlen(text[i]))== 0)
  return true;
 else
  return false;
}

static void get_cell_number(void)
{
 unsigned char i, j, k;
 
 //read reply cell number
 if (cstrncmp(text[33], tmp_rx0_msg, cstrlen(text[33]))== 0)
 {
  k= 0;
  j= 0;
  for (i= 12; i< strlen(tmp_rx0_msg); i++)
  {
   if ((tmp_rx0_msg[i]== ',') && (k== 0))
    k= i;
   else if ((tmp_rx0_msg[i]== ',') && (j== 0))
    j= i;
  }
  for (i= k+1; i<= j-1; i++)
   gprs.gsm.cell_number[i-k-1]= tmp_rx0_msg[i];
  gprs.gsm.cell_number[i-k-1]= ch_null;
 }
 else
 {
  cstrcpy(gprs.str, text[35]);
  sprintf(gprs.gsm.cell_number,"%c%s%c", ch_quote, gprs.str, ch_quote);
 }
}

static unsigned int str2int(char *ptr, unsigned char start, unsigned char stop)
{
 unsigned char i;
 unsigned int data= 0;
 char ok= true;
 
 for (i= start; i<= stop; i++)
  ok&= isdigit(ptr[i]);
 
 if (ok)
  for (i= start; i<= stop; i++)
  {
   data*= 10;
   data+= ptr[i] - '0';
  }
 
 return data;
}


void usartd0_tx_isr(void)
{
 //USART0, Tx Complete
 if (*tx0_ptr!= 0x00)
 {
  USARTD0.DATA = (*tx0_ptr);
  tx0_ptr++;
 }
 else
  tx0_fo= false;
}

void usartd0_write(char *ptr) //feed string into serial port
{
 static char tx0_msg[max_str_length];
 unsigned int i= 0;
 
 // Wait for empty transmit buffer 
 while (tx0_fo && (i++< 60000));
 
 strcpy(tx0_msg, ptr);
 tx0_ptr= tx0_msg;
 
 //send first char
 if (*tx0_ptr!= 0x00)
 {
  USARTD0.DATA = (*tx0_ptr);
  tx0_ptr++;
  tx0_fo= true;
 }
}

void usartd0_putch(char byte)
{
 while (tx0_busy);
 USARTD0.DATA = byte;
}

