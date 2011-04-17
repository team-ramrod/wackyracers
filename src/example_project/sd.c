#include "sd.h"

unsigned char sd_init(void)
{
 unsigned char i, response, sd_version;
 unsigned int retry=0 ;
 
// uart1_write("#1\r");
 
 for (i= 0; i< 10000; i++)
  write_spid(0xff);   //80 clock pulses spent before sending the first command
 
 sd_clock_and_release(); // clean up	
 sd_send_command(cmd_go_idle_state,0); // send CMD0 - reset card 
 sd_send_command(cmd_send_op_cond,0); // send CMD1 - activate card init 
 sd_card_cs_lo; // enable CS
 do
 {
  response = sd_send_command(cmd_go_idle_state, 0); //send 'reset & go idle' command
  retry++;
  if (retry> 0xff)
  {
   sd.ok= bit_false;
//   uart1_write("#2\r");
   return 1;   //time out, card not detected
  }
 } while(response!= 0x01);
 
 sd_clock_and_release(); // clean up
 
 sd_card_cs_lo; // enable CS
 
 sd_clock_and_release(); // clean up
 write_spid(0xff);
 write_spid(0xff);
 
 retry = 0;
 
 sd_version= 2; //default set to SD compliance with ver2.x; this may change after checking the next command
 
 do
 {
  response= sd_send_command(send_if_cond, 0x000001aa); //Check power supply status, mendatory for SDHC card
  retry++;
  if (retry> 0xfe)
  {
   sd_version= 1;
   file.card_type= 1;
   break;
  } //time out
 } while (response!= 0x01);
 
 retry= 0;
 
 do
 {
  response= sd_send_command(app_cmd, 0); //CMD55, must be sent before sending any ACMD command
  response= sd_send_command(sd_send_op_cond, 0x40000000); //ACMD41
  
  retry++;
  if (retry> 0xfe)
  {
   sd.ok= bit_false;
//   uart1_write("#3\r");
   return 2;  //time out, card initialization failed
  }
 } while (response!= 0x00);
 
 retry= 0;
 file.sdhc_flag= 0;
 
 if (sd_version== 2)
 {
  do
  {
   response= sd_send_command(read_ocr, 0);
   retry++;
   if (retry> 0xfe)
   {
    file.card_type= 0;
	break;
   } //time out
  } while (response!= 0x00);
  
  if (file.sdhc_flag== 1) 
   file.card_type= 2;
  else 
   file.card_type= 3;
 }
 
 //SD_sendCommand(CRC_ON_OFF, OFF); //disable CRC; deafault - CRC disabled in SPI mode
 //SD_sendCommand(SET_BLOCK_LEN, 512); //set block size to 512; default size is 512
 
 get_boot_sector_data();
 
 sd.ok= bit_true;
 //memory_statistics ();
 
// uart1_write("#4\r");
 return 0; //successful return
}


//******************************************************************
//Function	: to send a command to SD card
//Arguments	: unsigned char (8-bit command value)
// 			  & unsigned long (32-bit command argument)
//return	: unsigned char; response byte
//******************************************************************
unsigned char sd_send_command(unsigned char cmd, unsigned long arg)
{
 unsigned char response, retry=0, status;
 
 //SD card accepts byte address while SDHC accepts block address in multiples of 512
 //so, if it's SD card we need to convert block address into corresponding byte address by 
 //multipying it with 512. which is equivalent to shifting it left 9 times
 //following 'if' loop does that 
 
 if (file.sdhc_flag == 0)		
  if(cmd== read_single_block     ||
     cmd== read_multiple_blocks  ||
     cmd== write_single_block    ||
     cmd== write_multiple_blocks ||
     cmd== erase_block_start_addr|| 
     cmd== erase_block_end_addr ) 
  {
   arg = arg << 9;
  }	   
 
 sd_card_cs_lo; // enable CS 
 
 write_spid(cmd | 0x40); //send command, first two bits always '01'
 write_spid(arg>>24);
 write_spid(arg>>16);
 write_spid(arg>>8);
 write_spid(arg);
 
 if(cmd== send_if_cond)	 //it is compulsory to send correct CRC for CMD8 (CRC=0x87) & CMD0 (CRC=0x95)
  write_spid(0x87);    //for remaining commands, CRC is ignored in SPI mode
 else 
  write_spid(0x95); 
 
 while ((response= read_spid())== 0xff) //wait response
  if(retry++ > 0xfe) 
   break; //time out error
 
 if ((response== 0x00) && (cmd== 58))  //checking response of CMD58
 {
  status= read_spid() & 0x40;     //first byte of the OCR register (bit 31:24)
  if (status== 0x40) 
   file.sdhc_flag = 1;  //we need it to verify SDHC card
  else 
   file.sdhc_flag = 0;
 
  read_spid(); //remaining 3 bytes of the OCR register are ignored here
  read_spid(); //one can use these bytes to check power supply limits of SD
  read_spid(); 
 }
 
 read_spid(); //extra 8 CLK
 sd_card_cs_hi; // disable CS 
 
 return response; //return state
}

/** Finish Clocking and Release card.
    Send 10 clocks to the sd/SD card
    \and release the CS line 
*/
static void sd_clock_and_release(void)
{
 unsigned char i;
 
 // SD cards require at least 8 final clocks
 for(i= 0;i< 10; i++)
  write_spid(0xff);	
 
 sd_card_cs_hi; // release CS
}

//////////////////////////////////////////////////
///////////add to sd card modifications///////////
//////////////////////////////////////////////////
//*****************************************************************
//Function	: to erase specified no. of blocks of SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//*****************************************************************
unsigned char sd_erase(unsigned long start_block, unsigned long total_blocks)
{
 unsigned char response; 
 
 response= sd_send_command(erase_block_start_addr, start_block); //send starting block address
 if(response!= 0x00) //check for SD status: 0x00 - OK (No flags set)
  return response;
 
 response= sd_send_command(erase_block_end_addr,(start_block + total_blocks - 1)); //send end block address
 if(response!= 0x00)
  return response;
 
 response= sd_send_command(erase_selected_blocks, 0); //erase all selected blocks
 if(response != 0x00)
  return response;
 
 return 0; //normal return
}

//******************************************************************
//Function	: to read a single block from SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************
unsigned char sd_read_single_block(unsigned long start_block)
{
 unsigned char response;
 unsigned int i, retry=0;
 
 response = sd_send_command(read_single_block, start_block); //read a Block command
 
 if (response!= 0x00) 
  return response; //check for SD status: 0x00 - OK (No flags set)
 
 sd_card_cs_lo; // enable CS
 
 retry= 0;
 while (read_spid()!= 0xfe) //wait for start block token 0xfe (0x11111110)
  if(retry++> 0xfffe)
  {
   sd_card_cs_hi; 
   return 1;
  } //return if time-out
 
 for(i= 0; i< 512; i++) //read 512 bytes
  file.buffer_fat32[i] = read_spid();
 
 read_spid(); //receive incoming CRC (16-bit), CRC is ignored here
 read_spid();
 
 read_spid(); //extra 8 clock pulses
 sd_card_cs_hi; // disable CS
 
 return 0;
}

//******************************************************************
//Function	: to write to a single block of SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************
unsigned char sd_write_single_block(unsigned long start_block)
{
 unsigned char response;
 unsigned int i, retry=0;
 
 response= sd_send_command(write_single_block, start_block); //write a Block command
 
 if (response!= 0x00) 
  return response; //check for SD status: 0x00 - OK (No flags set)
 
 sd_card_cs_lo; // enable CS
 
 write_spid(0xfe);     //Send start block token 0xfe (0x11111110)
 
 for (i= 0; i< 512; i++)    //send 512 bytes data
  write_spid(file.buffer_fat32[i]);
 
 write_spid(0xff);     //transmit dummy CRC (16-bit), CRC is ignored here
 write_spid(0xff);
 
 response = read_spid();
 
 if ((response & 0x1f) != 0x05) //response= 0xXXX0AAA1 ; AAA='010' - data accepted
 {                              //AAA='101'-data rejected due to CRC error
  sd_card_cs_hi;              //AAA='110'-data rejected due to write error
  return response;
 }
 
 while (!read_spid()) //wait for SD card to complete writing and get idle
  if (retry++> 0xfffe)
  {
   sd_card_cs_hi; 
   return 1;
  }
 
 sd_card_cs_hi;
 write_spid(0xff);   //just spend 8 clock cycle delay before reasserting the CS line
 sd_card_cs_lo;;         //re-asserting the CS line to verify if card is still busy
 
 while(!read_spid()) //wait for SD card to complete writing and get idle
  if(retry++> 0xfffe)
  {
   sd_card_cs_hi; 
   return 1;
  }
 
 sd_card_cs_hi; // disable CS
 
 return 0;
}

