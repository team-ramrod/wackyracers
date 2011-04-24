#ifndef __ports_h
#define __ports_h
//// = changed
// = unchanged


#define bit_gprs_pwr 		bit(0)
#define bit_gprs_on 		bit(2)
#define bit_gprs_rts 		bit(3)
#define bit_gprs_reset 		bit(4)
#define bit_gprs_dtr 		bit(5)


// EN_HYD1 	bit(PF0)
// EN_HYD2		bit(PF1)
// EN_ANGLE	bit(PF4)
// EN_WAVE		bit(PF5)

#define bit_dataflash_wp	bit(2)
#define bit_dataflash_cs 	bit(3)
#define bit_dataflash_reset bit(4)
#define bit_dataflash_mosi	bit(5)
#define bit_dataflash_miso	bit(6)
#define bit_dataflash_sck	bit(7)
    
#define bit_sd_card_cs		bit(4)
#define bit_sd_card_mosi	bit(5)
#define bit_sd_card_miso	bit(6)
#define bit_sd_card_sck		bit(7)


#define sd_card_cs_lo PORTD.OUTCLR = bit_sd_card_cs
#define sd_card_cs_hi PORTD.OUTSET = bit_sd_card_cs
#define sd_card_cs_toggle PORTD.OUTTGL = bit_sd_card_cs
#define sd_card_cs (PORTD.IN &bit_sd_card_cs)



#define dataflash_wp_lo PORTE.OUTCLR = bit_dataflash_wp
#define dataflash_wp_hi PORTE.OUTSET = bit_dataflash_wp
#define dataflash_wp_toggle PORTE.OUTTGL = bit_dataflash_wp
#define dataflash_wp (PORTE.IN &bit_dataflash_wp)

#define dataflash_cs_lo PORTE.OUTCLR = bit_dataflash_cs
#define dataflash_cs_hi PORTE.OUTSET = bit_dataflash_cs
#define dataflash_cs_toggle PORTE.OUTTGL = bit_dataflash_cs
#define dataflash_cs (PORTE.IN &bit_dataflash_cs)

#define dataflash_reset_lo PORTE.OUTCLR = bit_dataflash_reset
#define dataflash_reset_hi PORTE.OUTSET = bit_dataflash_reset
#define dataflash_reset_toggle PORTE.OUTTGL = bit_dataflash_reset
#define dataflash_reset (PORTE.IN &bit_dataflash_reset)



#define gprs_pwr_lo PORTD.OUTCLR = bit_gprs_pwr
#define gprs_pwr_hi PORTD.OUTSET = bit_gprs_pwr
#define gprs_pwr_toggle PORTD.OUTTGL = bit_gprs_pwr
#define gprs_pwr (PORTD.IN &bit_gprs_pwr)

#define gprs_on_lo PORTC.OUTCLR = bit_gprs_on
#define gprs_on_hi PORTC.OUTSET = bit_gprs_on
#define gprs_on_toggle PORTC.OUTTGL = bit_gprs_on
#define gprs_on (PORTC.IN &bit_gprs_on)

#define gprs_rts_lo PORTC.OUTCLR = bit_gprs_rts
#define gprs_rts_hi PORTC.OUTSET = bit_gprs_rts
#define gprs_rts_toggle PORTC.OUTTGL = bit_gprs_rts
#define gprs_rts (PORTC.IN &bit_gprs_rts)

#define gprs_reset_lo PORTC.OUTCLR = bit_gprs_reset
#define gprs_reset_hi PORTC.OUTSET = bit_gprs_reset
#define gprs_reset_toggle PORTC.OUTTGL = bit_gprs_reset
#define gprs_reset (PORTC.IN &bit_gprs_reset)

#define gprs_dtr_lo PORTC.OUTCLR = bit_gprs_dtr
#define gprs_dtr_hi PORTC.OUTSET = bit_gprs_dtr
#define gprs_dtr_toggle PORTC.OUTTGL = bit_gprs_dtr
#define gprs_dtr (PORTC.IN &bit_gprs_dtr)


#endif
