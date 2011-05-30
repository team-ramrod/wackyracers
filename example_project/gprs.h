#ifndef __gprs_h
#define __gprs_h

#include "common.h"

#define gprs_timeout_1min 600
#define gprs_timeout_3min 1800

//extern prototypes
extern char ee_write(unsigned int location, unsigned int data, unsigned char bytes, unsigned char real_time);

//extern variables
extern uint8_t df;

//local prototypes
void gprs_init(void);
void gprs_idle(void);
void gprs_power_up(void);
void gprs_gsm_gps(void);
void gprs_power_down(void);
void gprs_gsm_send(void);
void gprs_gsm_inbox(void);
void gprs_email_send(void);
static void create_gsm_msg(void);
static void check_wait_states(int *i);
static void set_quotes(char *ptr);



#endif
