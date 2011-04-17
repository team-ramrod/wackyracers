#include "stdint.h"

//struct

#define max_str_length 128

struct
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
 float water_pressure;      
 float load_current_1;       
 float load_current_2;       
 float hydraulic_pressure_1; 
 float hydraulic_pressure_2;
 float arm_angle;
} parameters;
struct
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
struct
{
 unsigned close_1: 1,
          close_2: 1;
} control;
struct
{
 long joules;
 float peak_kw;
 long sample_joules;
 long samples;
} energy;
struct
{
 char ok;
 char recall;
 unsigned char delay;
 unsigned long sector;
 unsigned long start_sector;
 unsigned long finish_sector;
} sd;
struct
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
struct
{
 unsigned char hour;
 unsigned char minute;
 unsigned char second;
 unsigned char tenth_second;
 unsigned char day;
 unsigned char month;
 unsigned char year;
} date_time;
struct
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
void (*gprs_ptr)(void);

#define MAX_INTERRUPTS 6 

// Interrupt Control
interrupt_t interrupts[MAX_INTERRUPTS];
uint8_t num_interrupts;



// ex-local variables from gprs.h
struct
{
 int power_down;
 int gsm_gps;
 int gsm_send;
 int gsm_inbox;
 int email_send;
} gprs_index;
char try_alternative_email_address;/*
__flash static char text[][55]= 
{
 "ATE0\r", //0
 "AT#SERVINFO\r", //1
 "\rAT\r", //2
 "AT+CFUN=1\r", //3
 "AT+CGDCONT= 1,'IP','internet','0.1.1.0',0,0\r", //4
 "AT#USERID= 'guest'\r", //5
 "AT#PASSW= 'guest'\r", //6 
 "AT#PKTSZ= 300\r", //7
 "AT#DSTO= 50\r", //8
 "AT#SKTTO= 900\r", //9
 "AT#SKTCT= 600\r", //10
 "AT#SKTSAV\r", //11
 "AT#GPRS= 1\r", //12
 "AT#SKTD= 0,25,'smtp.vodafone.net.nz',0,0\r", //13
 "HELO\r\n", //14
 "MAIL FROM: psaf.device\r\n", //15
 "RCPT TO: psaf.device@gmail.com\r\n", //16
 "DATA\r\n", //17
 "From: <PSAF device>\r\n", //18
 "To: <psaf.device@gmail.com>\r\n", //19
 "Subject: PSAF Device Data\r\n", //20
 "\r\n", //21
 "\r\n.\r\n", //22
 "QUIT\r\n", //23
 "+++\r\n", //24
 "AT+CMGF=1\r", //25
 ",", //26
 "AT$GPSP=1\r", //27
 "AT$GPSR=1\r", //28
 "AT$GPSACP\r", //29
 "AT$GPSP=0\r", //30
 ",,", //31
 "AT+CSCA?\r", //32
 "AT+CMGS=", //33
 "AT+CGDCONT = 1,'IP','internet.gprs','0.0.0.0',0,0\r", //34
 "AT#SCFG=1,1,300,90,600,50\r", //35
 "AT#SGACT=1,1,'EASY GPRS','EASY GPRS'\r", //36
 "AT#SD=1, 0,25,'smtp2.vodafone.net.nz',0,0\r", //37 
 "HELO pcprova\r\n", //38
 "MAIL FROM: psaf.device@vodafone.net.nz \r\n", //39
 "RCPT TO: psaf.device@gmail.com \r\n", //40
 "AT#SH=1\r", //41
 "RCPT TO: psaf.device@vodafone.net.nz \r\n", //42
 "", //43
 "GPRS Hours ", //44
 "Software 7v50\rDate,Time,", //45 
 "+OK ", //46
 "AT$GPSPS=0\r", //47
 "AT+CMGD=1,4\r", //48
 "AccelX,AccelY,", //49
 "BatVoltage,", //50
 "GenVoltage1,GenVoltage2,", //51
 "LoadCrt1,LoadCrt2,", //52
 "WaveHeight,", //53
 "Hydraulic1,Hydraulic2,", //54
 "+CMS ERROR: 321", //55
 "OK", //56
 "220", //57
 "250", //58
 "354", //59
 "221", //60
 "CONNECT", //61
 "ERROR", //62
 "-ERR", //63
 "", //64
 "HousingTemp,", //65
 "", //66
 "", //67
 "", //68
 "", //69
 "AT+CMGL='REC UNREAD'\r", //70
 "", //71
 "", //72
 "", //73
 "", //74
 "", //75
 "", //76
 "+CMGS", //77
 "\r\nPSAF Device\r", //78
 "Relay 1 closed\r", //79
 "Pages ", //80
 "Angle,", //81
 "Energy ", //82
 "Peak ", //83
 "Sample Joules ", //84
 "", //85
 "Relay 1 open\r", //86
 "Relay 2 closed\r", //87
 "Relay 2 open\r", //88
 "", //89
 "", //90
 "WaterTemp,", //91
 "", //92
 "NO CARRIER", //93
 "", //94
 "", //95
 "", //96
 "", //97
 "", //98
 "", //99
 "", //100
 "", //101
 "", //102
 "Relay1,Relay2,", //103
};*/
__flash char gprs_text[][55]= 
{
 "ATE0\r", //0
 "AT#SERVINFO\r", //1
 "\rAT\r", //2
 "AT+CFUN=1\r", //3
 "AT+CGDCONT= 1,'IP','internet','0.1.1.0',0,0\r", //4
 "AT#USERID= 'guest'\r", //5
 "AT#PASSW= 'guest'\r", //6 
 "AT#PKTSZ= 300\r", //7
 "AT#DSTO= 50\r", //8
 "AT#SKTTO= 900\r", //9
 "AT#SKTCT= 600\r", //10
 "AT#SKTSAV\r", //11
 "AT#GPRS= 1\r", //12
 "AT#SKTD= 0,25,'smtp.vodafone.net.nz',0,0\r", //13
 "HELO\r\n", //14
 "MAIL FROM: psaf.device\r\n", //15
 "RCPT TO: psaf.device@gmail.com\r\n", //16
 "DATA\r\n", //17
 "From: <PSAF device>\r\n", //18
 "To: <psaf.device@gmail.com>\r\n", //19
 "Subject: PSAF Controller Test\r\n", //20
 "\r\n", //21
 "\r\n.\r\n", //22
 "QUIT\r\n", //23
 "+++\r\n", //24
 "AT+CMGF=1\r", //25
 ",", //26
 "AT$GPSP=1\r", //27
 "AT$GPSR=1\r", //28
 "AT$GPSACP\r", //29
 "AT$GPSP=0\r", //30
 ",,", //31
 "AT+CSCA?\r", //32
 "AT+CMGS=", //33
 "AT+CGDCONT = 1,'IP','internet.gprs','0.0.0.0',0,0\r", //34
 "AT#SCFG=1,1,300,90,600,50\r", //35
 "AT#SGACT=1,1,'EASY GPRS','EASY GPRS'\r", //36
 "AT#SD=1, 0,25,'smtp2.vodafone.net.nz',0,0\r", //37 
 "HELO pcprova\r\n", //38
 "MAIL FROM: psaf.device@vodafone.net.nz \r\n", //39
 "RCPT TO: psaf.device@gmail.com \r\n", //40
 "AT#SH=1\r", //41
 "RCPT TO: psaf.device@vodafone.net.nz \r\n", //42
 "", //43
 "GPRS Hours ", //44
 "Software 7v50\rDate,Time,", //45 
 "+OK ", //46
 "AT$GPSPS=0\r", //47
 "AT+CMGD=1,4\r", //48
 "AccelX,AccelY,", //49
 "BatVoltage,", //50
 "GenVoltage1,GenVoltage2,", //51
 "LoadCrt1,LoadCrt2,", //52
 "WaveHeight,", //53
 "Hydraulic1,Hydraulic2,", //54
 "+CMS ERROR: 321", //55
 "OK", //56
 "220", //57
 "250", //58
 "354", //59
 "221", //60
 "CONNECT", //61
 "ERROR", //62
 "-ERR", //63
 "", //64
 "HousingTemp,", //65
 "", //66
 "", //67
 "", //68
 "", //69
 "AT+CMGL='REC UNREAD'\r", //70
 "", //71
 "", //72
 "", //73
 "", //74
 "", //75
 "", //76
 "+CMGS", //77
 "\r\nPSAF Device\r", //78
 "Relay 1 closed\r", //79
 "Pages ", //80
 "Angle,", //81
 "Energy ", //82
 "Peak ", //83
 "Sample Joules ", //84
 "", //85
 "Relay 1 open\r", //86
 "Relay 2 closed\r", //87
 "Relay 2 open\r", //88
 "", //89
 "", //90
 "WaterTemp,", //91
 "", //92
 "NO CARRIER", //93
 "", //94
 "", //95
 "", //96
 "", //97
 "", //98
 "", //99
 "", //100
 "", //101
 "", //102
 "Relay1,Relay2,", //103
};
