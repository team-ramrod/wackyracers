#ifndef __timer_h
#define __timer_h

#pragma interrupt_handler timer_ovf_isr:iv_TCC0_OVF

#define read 0


//extern prototypes
void gprs_email_send(void);
void maintain_ee(void);
void get_date(void);
void get_time(void);
void save_to_sd(void);
void save_to_df(void);
int  adc_conversion(unsigned char ch, unsigned char reference);
void talk_to_external_sensors(void);
void timer10(void);
void timer50(void);
void timer100(void);
void timer200(void);
void timer_init(void);
void timer_ovf_isr(void);

#define BUFFER_LENGTH 200


#pragma interrupt_handler timer_ovf_isr:iv_TCC0_OVF

#endif

