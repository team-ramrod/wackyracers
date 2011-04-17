/** @file   micro.c
 *  @author Simon Richards
 *  @date   20 December 2010
 *
 *	System initiation and sleep function
 *
 *  Status: Not tested
*/

#include "common.h"

void port_init(void)
{
 PORTA.DIR = 0x00; // ADC ports set as inputs
 PORTB.DIR = 0x00; 
 
 PORTC.DIR = 0xbf; // RTC and GPRS control lines set to out, GR1 RXD set as in
 PORTC.OUT = 0x00;
 
 PORTD.DIR = 0xbf; // SD control bits, UART0 and GR1 PWR set as outputs, SD MISO as input
 PORTD.OUT = 0x00;
 
 PORTE.DIR = 0xbf; // DF control lines set as outputs, DF MISO as input
 PORTE.OUT = 0x00;
 
 PORTF.DIR = 0xfb; // 4-20mA enable lines set as outputs, DV RXD set as input
 PORTF.OUT = 0x00;
 
 dataflash_cs_hi;
 dataflash_reset_hi;
 sd_card_cs_hi;
}


uint8_t attach_interrupt(void (*func)(void)){
 interrupts[num_interrupts].flag = 0;
 interrupts[num_interrupts].function = func; 
 return num_interrupts++;
}

void interrupts_init(void){ 
 //Enable Low/Med/High level Interrupts and 
 //enable Round Robin prioritising for Low Level Interrupts 
 PMIC.CTRL = bit(0) | bit(1) | bit(2) | bit(7);
}


//Watchdog initialize
// prescale: 128K 
void watchdog_init(void)
{
 reset_watchdog; //this prevents a timout on enabling
 WDT.CTRL= 0x0b; //WATCHDOG ENABLED at ~32ms - dont forget to issue WDRs
}

void clock_init(void)
{
    OSC.CTRL |= OSC_RC32MEN_bm; //enable 32Mhz clock
    while ((OSC.STATUS & OSC_RC32MRDY_bm) == 0); // wait for stability
    CPU.CCP = CCP_IOREG_gc;
    CLK.CTRL = 0x01; //set 32Mhz clock as system clock
    CPU.CCP = CCP_IOREG_gc;
    CLK.PSCTRL = 0x00; // no prescaling
}

 
/**
 * Sleeps the CPU until the next interrupt
 */
void sleep_until_interrupt(void){
    SLEEP.CTRL = 0x0f; // Enable sleep, extended standby mode
    sleep();           // Enter sleep mode
    SLEEP.CTRL = 0x00; // Disable sleep mode
}

