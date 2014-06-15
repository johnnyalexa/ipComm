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
#include <stdio.h>


#define USE_LOGGING			(0xFF)

#ifdef USE_LOGGING
#define SYS_LOG(...)	printf(__VA_ARGS__)
#else
#define SYS_LOG(...)	{}
#endif //USE_LOGGING



typedef struct ipComm_config{
	uint8_t			local_mac[6];	//mac address of current device
	uint8_t			server_ip[4];	//server ip address to report 
	uint16_t		server_port;	//tcp server port
	uint8_t			status;			//status value for eeprom loop
}ipComm_config_t;


// Reset AVR using watchdog
#define Reset_AVR() wdt_enable(WDTO_500MS); while(1) {}

void timer_init(void);
void Ethernet_Init(void);
void MCU_Init(void);

void Init_Uart(void);
void USART_Transmit(uint8_t data);
uint8_t USART_Receive(uint8_t * data);
void USART_print(char * text);

/* ******* NVM ***************** */
uint8_t NVM_GetCurrentPosition(void);
int NVM_LoadConfig(ipComm_config_t *data);
void NVM_SaveConfig(ipComm_config_t *data);

/* ******* GPIO ***************** */
// Led and button control
#define STATUS_LED_PORT		PORTD
#define STATUS_LED			(1<<PD6)
#define RESET_SW_PORT		PIND
#define RESET_SW_PIN		(1<<PIND7)

#define STATUS_ON()		STATUS_LED_PORT|=STATUS_LED
#define STATUS_OFF()	STATUS_LED_PORT&=~STATUS_LED
#define STATUS_TOGGLE()	STATUS_LED_PORT^=STATUS_LED

#define RESET_SW_IS_PUSHED()	!(RESET_SW_PORT&RESET_SW_PIN)

void GPIO_init(void);
int GetResetSw(void);

#endif /* DRIVERS_H_ */