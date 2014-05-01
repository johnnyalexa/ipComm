/*
 * drivers.h
 *
 * Created: 4/30/2014 10:14:19 PM
 *  Author: John
 */ 


#ifndef DRIVERS_H_
#define DRIVERS_H_

#include <avr/io.h>
#include <avr/wdt.h>
// Reset AVR using watchdog
#define Reset_AVR() wdt_enable(WDTO_500MS); while(1) {}

void timer_init(void);
void Ethernet_Init(void);
void MCU_Init(void);

void Init_Uart(void);
void USART_Transmit(uint8_t data);
uint8_t USART_Receive(uint8_t * data);
void USART_print(char * text);



#endif /* DRIVERS_H_ */