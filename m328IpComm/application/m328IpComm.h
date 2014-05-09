/*
 * m328IpComm.h
 *
 * Created: 4/30/2014 10:43:03 PM
 *  Author: John
 */ 


#ifndef M328IPCOMM_H_
#define M328IPCOMM_H_
#include "../drivers/drivers.h"
#include "../enc28j60_tcp_ip_stack/include/tcp_ip_stack.h"

typedef struct protocol_config{
	uint8_t ip_protocol;
	
	}protocol_config_t;


#define WEBSERVER_VHOST "senso-trafic.no-ip.org"

// packet buffer
#define BUFFER_SIZE 650

//// listen port for udp
#define MYUDPPORT 4601
#define MYTCPPORT 55056
#define MYWWWPORT 80

#define TRANS_NUM_GWMAC 1
#define TRANS_NUM_WEBMAC 2

extern uint8_t netmask[4];
extern uint8_t mymac[6];
extern uint8_t gwmac[6];
extern uint8_t otherside_www_ip[4];
extern uint8_t otherside_www_gwmac[6];
// My own IP (DHCP will provide a value for it):
extern uint8_t myip[4];
// Default gateway (DHCP will provide a value for it):
extern uint8_t gwip[4];

extern volatile uint8_t sec;
extern volatile uint8_t gsec;
// packet buffer
extern uint8_t buf[BUFFER_SIZE+1];

void arpresolver_result_callback(uint8_t *ip __attribute__((unused)),uint8_t reference_number,uint8_t *mac);
uint8_t your_client_tcp_result_callback(uint8_t fd, uint8_t statuscode,uint16_t data_start_pos_in_buf, uint16_t len_of_data);
uint16_t your_client_tcp_datafill_callback(uint8_t fd);
void send_tcp_data(void);
void Ethernet_Leds_Init(void);
uint16_t Get_DHCP_Config(void);

void clear_buf(uint16_t len);

//// webserver 1

uint8_t verify_password(char *str);
uint16_t http200ok(uint8_t * buf);
uint16_t print_webpage_config(uint8_t * buf);
uint16_t print_webpage(uint8_t * buf);

uint16_t print_webpage_login(uint8_t * buf);

int8_t analyse_get_url(uint8_t * buf, char *str);


#endif /* M328IPCOMM_H_ */