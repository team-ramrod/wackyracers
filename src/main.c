/** @file   main.c
 *  @author Simon Richards
 *  @date   20 December 2010 modified: 18/1/2011
 *
 *	 Initiation calls and main loop.
 *
 *  Interrupts set flags for the cooperative scheduler
 *  which is run in the main loop. Add tasks using the
 *  attach interrupt function.
 *
 *  Status: Untested
*/

#include "common.h"

void main(void)
{
 int i;
 
 clock_init();
 port_init();
 fat32_init();
 timer_init();
 spie_init();
 spid_init();
 usartd0_init();
 usartc1_init();
 adc_init();
 twic_init();
 rtc_init();
 compass_init();
 ee_init();
 gprs_init();
 SEI();
 
 dataflash_init();
 sd_init();
 watchdog_init();
 
 while (true)
 {
  for (i = 0; i < num_interrupts; i++)
   if (interrupts[i].flag)
    interrupts[i].function();
  sleep_until_interrupt();
 }
}
