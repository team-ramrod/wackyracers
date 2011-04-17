/** @file   adc.c
 *  @author Simon Richards
 *  @date   20 December 2010, modified 18/1/2011
 *
 *	ADC methods and interrupts
 *  
 *  adc_start triggers an ADC sweep, values are saved and 
 *  the input channels are automatically changed. 
 *
 *  Status: Not tested
*/

#include "common.h"
#include "adc.h"

typedef enum {groupA, groupB} state;
static state channel_state = groupA;
static char writeBuffer[BUFFER_LENGTH];

void adc_init(void){
 // Setting pins to input
 PORTA.DIRCLR = 0xff; 
 PORTB.DIRCLR = 0xff; 
 
 // Setting up ADC Registers
 ADCA.CTRLA = 0x01;        // No DMA, start no channels, no flush, enable ADC
 ADCA.CTRLB = 0x00;        // Unsigned, non freerunning, 12 bit resolution
 ADCA.REFCTRL = 0x00;      // Internal 1.00V reference
 ADCA.EVCTRL = 0x00;       //No event inputs
 ADCA.PRESCALER = 0x00;    // Clk/4
 ADCA.CH0.CTRL = ADC_GAIN; //1x gain, single ended
 ADCA.CH1.CTRL = ADC_GAIN;
 ADCA.CH2.CTRL = ADC_GAIN; 
 ADCA.CH3.CTRL = ADC_GAIN; 
 ADCB.CTRLA = ADCA.CTRLA;  // Identical set up for ADCB
 ADCB.CTRLB = ADCA.CTRLB; 
 ADCB.REFCTRL = ADCA.REFCTRL; 
 ADCB.EVCTRL = ADCA.EVCTRL; 
 ADCB.PRESCALER = ADCA.PRESCALER; 
 ADCB.CH0.CTRL = ADC_GAIN; 
 ADCB.CH1.CTRL = ADC_GAIN;
 ADCB.CH2.CTRL = ADC_GAIN; 
 ADCB.CH3.CTRL = ADC_GAIN; 
 
}

/**
 * Switches ADC channels between first and second set
 */
void adc_change_state(void)
{
 if (channel_state == groupB)
 {
  channel_state = groupA;
  ADCA.CH0.MUXCTRL = 0x00; //ADC0
  ADCA.CH1.MUXCTRL = 0x08; //ADC1
  ADCA.CH2.MUXCTRL = 0x10; //ADC2 

  ADCB.CH0.MUXCTRL = 0x00; //ADC0
  ADCB.CH1.MUXCTRL = 0x08; //ADC1
  ADCB.CH2.MUXCTRL = 0x10; //ADC2
  ADCB.CH3.MUXCTRL = 0x08; //ADC3
 }
 else
 {
  channel_state = groupB;
  ADCA.CH0.MUXCTRL = 0x18; //ADC3
  ADCA.CH1.MUXCTRL = 0x20; //ADC4
  ADCA.CH2.MUXCTRL = 0x31; //ADC7
  
  ADCB.CH0.MUXCTRL = 0x20; //ADC4
  ADCB.CH1.MUXCTRL = 0x21; //ADC5
  ADCB.CH2.MUXCTRL = 0x30; //ADC6
  ADCB.CH3.MUXCTRL = 0x31; //ADC7 
 }
}

/**
 * Initiates conversions for all channels
 * Call this function only as the saving of values is handled through interrupts
 */
void adc_start(void){
 adc_change_state();
 ADCA.CTRLA = 0x1A; // Start channels 0:2
 ADCB.CTRLA = 0x3A; // Start channels 0:3
}

#define A0 0.7      // Set filter level here
#define A1 1 - A0
#define update(param,channel,coeff,const) parameters.param=(parameters.param*A1)+(((float)channel.i*coeff+const)*A0)

/**
 * Adjusts and records sensor values
 */
void adc_save_data(void)
{
 static uint8_t length = 0;
 if (channel_state == groupA)
 { 
  update(acceleration_x,       ADCA.CH0RES, K_AX,O_AX);
  update(acceleration_y,       ADCA.CH1RES, K_AY,O_AY);
  update(acceleration_z,       ADCA.CH2RES, K_AZ,O_AZ);
  
  update(water_pressure,       ADCB.CH0RES, K_WAVE,0);
  update(hydraulic_pressure_1, ADCB.CH1RES, K_HYD1,0);
  update(hydraulic_pressure_2, ADCB.CH2RES, K_HYD2,0);
  update(load_current_2,       ADCB.CH3RES, K_CRT2,0);
 }
 else // groupB
 { 
  update(gyro_x,               ADCA.CH0RES, K_GX,0);
  update(gyro_y,               ADCA.CH1RES, K_GY,0);
  update(arm_angle,            ADCA.CH2RES, K_ANGLE,0);
  
  update(load_current_1,       ADCB.CH0RES, K_CRT1,0);
  update(generator_voltage_2,  ADCB.CH1RES, K_VGEN2,0);
  update(generator_voltage_1,  ADCB.CH2RES, K_VGEN1,0);
  update(battery_voltage,      ADCB.CH3RES, K_VBAT,0);
 }
}


void finished(void){
 static uint8_t i = 0;
 if (i++)
 {
  i = 0;
  adc_save_data();
  if (channel_state == groupA)
   adc_start();
 }
}

void ADCA_CH2_isr(void)
{
 finished();
}



void ADCB_CH3_isr(void)
{
 finished();
}



/**
 * Integrate the measurement of power over time
 */
void accumulate_power(void)
{
 float kw;
 
 kw= max(0, parameters.generator_voltage_1 * parameters.load_current_1);
 kw+= max(0, parameters.generator_voltage_2 * parameters.load_current_2);
 kw/= 1000;
 
 if (kw> energy.peak_kw)
  energy.peak_kw= kw;
 
 kw*= 10;
 energy.sample_joules+= (long) kw;
 energy.samples++;
 
 energy.joules+= (long) kw;
 if (energy.joules< 0)
  energy.joules= 0;
}

