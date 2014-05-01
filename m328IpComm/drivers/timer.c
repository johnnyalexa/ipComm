/*
 * timer.c
 *
 * Created: 4/30/2014 10:15:48 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "drivers.h"
#include "../application/m328IpComm.h"

#ifndef _OLD_MAIN_
// timer interrupt, called automatically every second
ISR(TIMER1_COMPA_vect){
	sec++;
	gsec++;
	if (sec>5){
		sec=0;
		dhcp_6sec_tick();
	}
}

// Generate an interrup about ever 1s form the 12.5MHz system clock
// Since we have that 1024 prescaler we do not really generate a second
// (1.00000256000655361677s)
void timer_init(void){
	/* write high byte first for 16 bit register access: */
	TCNT1H=0;  /* set counter to zero*/
	TCNT1L=0;
	// Mode 4 table 14-4 page 132. CTC mode and top in OCR1A
	// WGM13=0, WGM12=1, WGM11=0, WGM10=0
	TCCR1A=(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11);
	TCCR1B=(1<<CS12)|(1<<CS10)|(1<<WGM12)|(0<<WGM13); // crystal clock/1024
	// divide clock by 1024: 12.5MHz/1024=12207.0313 Hz

	// At what value to cause interrupt. You can use this for calibration
	// of the clock. Theoretical value for 12.5MHz: 12207=0x2f and 0xaf
	OCR1AH=0x2f;
	OCR1AL=0xaf;
	// interrupt mask bit:
	//TIMSK1 for atmega88
	TIMSK1 = (1 << OCIE1A);
}
#endif