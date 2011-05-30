#ifndef adc_h
#define adc_h


#include "common.h"

//#pragma interrupt_handler ADCA_CH0_isr:iv_ADCA_CH0
//#pragma interrupt_handler ADCA_CH1_isr:iv_ADCA_CH1
#pragma interrupt_handler ADCA_CH2_isr:iv_ADCA_CH2

//#pragma interrupt_handler ADCB_CH0_isr:iv_ADCB_CH0
//#pragma interrupt_handler ADCB_CH1_isr:iv_ADCB_CH1
//#pragma interrupt_handler ADCB_CH2_isr:iv_ADCB_CH2
#pragma interrupt_handler ADCB_CH3_isr:iv_ADCB_CH3

#define BUFFER_LENGTH 200

typedef enum {DATAFLASH_OK, DATAFLASH_FULL} save_data_result;

extern uint8_t conversion_complete_flg;

void adc_init(void);
void accumulate_power(void);
void adc_start(void);
#endif
