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



#define USE_DEFAULT         (0xFF)
#define USE_STATIC 			(0x0A)
#define USE_DHCP			(0x50)

typedef struct eth_config{
	uint8_t local_mac[6];
	uint8_t local_ip[4];
	uint8_t netmask[4];
	uint8_t local_gw[4];
}eth_config_t;

typedef struct disp_config{
	uint8_t  ip[4];
	uint16_t port;
	uint8_t  disp_dns[30];
}disp_config_t;

typedef struct ipComm_config{
	eth_config_t	net;
	disp_config_t	disp;
	uint8_t			status;
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
void NVM_LoadConfig(ipComm_config_t *data);
void NVM_SaveConfig(ipComm_config_t *data);

#endif /* DRIVERS_H_ */