/*
 * MCU.c
 *
 * Created: 4/30/2014 11:04:31 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "drivers.h"
#include "../enc28j60_tcp_ip_stack/include/timeout.h"

//
//Set MCU clock options
inline void MCU_Clk_Init(void){
	// Set the clock speed to "no pre-scaler" (8MHz with internal osc or
	// full external speed)
	// set the clock prescaler. First write CLKPCE to enable setting
	// of clock the next four instructions.
	// Note that the CKDIV8 Fuse determines the initial
	// value of the CKKPS bits
	CLKPR = (1<<CLKPCE);  // change enable
	CLKPR = 0;            // "no pre-scaler"
}

//
// Initialize MCU
void MCU_Init(void){
	cli();
	// Set Clk Option
	MCU_Clk_Init();
	// Wait Clk to establish
	_delay_loop_1(0); // 60us
	GPIO_init();
	//Init ENC28J60 ethernet controller
	Ethernet_Init();
	//Init timer
	timer_init();
	//Init UART
	Init_Uart();
	//Enable interrupts
	sei();
}

