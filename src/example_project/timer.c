#include "common.h"

uint8_t timer;

void timer_init(void)
{
 // Timer 0 generates events at the required intervals
 TCC0.CTRLA = 0x06; // clk/256
 TCC0.CTRLB = 0x00; // WGM == normal
 TCC0.CTRLC = 0x00; // CMPx all cleared
 TCC0.CTRLD = 0x00; // No events
 TCC0.CTRLE = 0x00; // 16 bit mode
 TCC0.CNT.i = 0xfb1d; // 10ms left on the clock
 TCC0.INTCTRLA = 0x02;
 // Timer 1 runs a PWM at 31kHz
 
 timer = attach_interrupt(timer10);
}

/*
 * Overflow ISR, timer is reset to start value: 0xfb1d
 */
void timer_ovf_isr(void)
{
 TCC0.CNT.i = 0xfb1d;
 
 reset_watchdog;
 
 interrupts[timer].flag = bit_true;
}


/*
 * 10 millisecond timer
 */
void timer10(void)
{
 static int i = 0;
 i = i < 4 ? i + 1 : 0;
 if (!i)
  timer50();
 interrupts[timer].flag = bit_false;
 
 
 //talk_to_external_sensors();
 accumulate_power();
 
}

/*
 * 50 millisecond timer
 */
void timer50(void)
{
 static int i = 0;
 i = i < 1 ? i + 1 : 0;
 if (!i)
  timer100();
 
 
 maintain_ee();
 save_to_df();
 update_time_and_date();
 
 if (dataflash.delay)
  dataflash.delay--;
}


/*
 * 100 millisecond timer
 */
void timer100(void)
{
 static int i = 0;
 i = i < 1 ? i + 1 : 0;
 if (!i)
  timer200();
 
 gprs_ptr();
}


/*
 * 200 millisecond timer
 */
void timer200(void)
{
 static int i = 0;
 
 adc_start();
 if (++i >= (int)(dataflash.gprs_hours) * 1800 && gprs.flags.idle)
 {
  gprs_ptr= gprs_email_send;
  i= 0;
 } 
}

