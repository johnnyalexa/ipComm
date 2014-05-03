/*
 * CommIpHelp.c
 *
 * Created: 4/30/2014 10:54:54 PM
 *  Author: John
 */ 
#include "m328IpComm.h"
#include "../enc28j60_tcp_ip_stack/include/timeout.h"
#include <avr/wdt.h>

//
// Please modify the following lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:       x     S    L    F    x   x
uint8_t mymac[6] = {0x00, 0x52,0x47,0x46,0x10,0x27};


//uint8_t otherside_www_ip[4]={192,168,0,100}; // dns will fill this
	uint8_t otherside_www_ip[4]={86,121,130,52}; // dns will fill this
// My own IP (DHCP will provide a value for it):
uint8_t myip[4]={0,0,0,0};
// Default gateway (DHCP will provide a value for it):
uint8_t gwip[4]={0,0,0,0};
#define TRANS_NUM_GWMAC 1
uint8_t gwmac[6];
#define TRANS_NUM_WEBMAC 2
uint8_t otherside_www_gwmac[6];
//static uint8_t otherside_www_gwmac[6]={0x78,0x92,0x9C,0x43,0x53,0x4A};
// Netmask (DHCP will provide a value for it):
uint8_t netmask[4];
char urlvarstr[32];	
	
volatile uint8_t sec=0; // counts up to 6 and goes back to zero
volatile uint8_t gsec=0; // counts up beyond 6 sec	


// packet buffer
uint8_t buf[BUFFER_SIZE+1];



// the __attribute__((unused)) is a gcc compiler directive to avoid warnings about unsed variables.
void arpresolver_result_callback(uint8_t *ip __attribute__((unused)),uint8_t reference_number,uint8_t *mac){
	uint8_t i=0;
	if (reference_number==TRANS_NUM_GWMAC){
		// copy mac address over:
		while(i<6){gwmac[i]=mac[i];i++;}
	}
	if (reference_number==TRANS_NUM_WEBMAC){
		// copy mac address over:
		while(i<6){otherside_www_gwmac[i]=mac[i];i++;}
	}
}

//
// Declare a callback function to get the result (tcp data from the server):
uint8_t your_client_tcp_result_callback(uint8_t fd, uint8_t statuscode,uint16_t data_start_pos_in_buf, uint16_t len_of_data){
	// Do not close the tcp connection after sending the packet
	uint8_t close_tcp_session = 0;
	//statuscode=0 means the buffer has valid data
	if(statuscode==0){
	}
	return(close_tcp_session);
}

//
// Declare a callback function to be called in order to fill in the
// request (tcp data sent to the server):
uint16_t your_client_tcp_datafill_callback(uint8_t fd){
	//no data has been loaded yet
	uint16_t len_of_data_filled_in = 0;
	return(fill_tcp_data_p(buf,0,PSTR("SUCCESS")));
	return(len_of_data_filled_in);
}


void send_tcp_data(void){
	uint8_t fd;
	//get_mac_with_arp(otherside_www_ip,TRANS_NUM_WEBMAC,&arpresolver_result_callback);
	fd=client_tcp_req(&your_client_tcp_result_callback,&your_client_tcp_datafill_callback,MYTCPPORT,otherside_www_ip,gwmac);
}

void Ethernet_Init(void){
	/*initialize enc28j60*/
	enc28j60Init(mymac);
	enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
	_delay_loop_1(0); // 60us
	
}

void Ethernet_Leds_Init(void){
	/* Magjack leds configuration, see enc28j60 datasheet, page 11 */
	// LEDB=yellow LEDA=green
	//
	// 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
	// enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
	enc28j60PhyWrite(PHLCON,0x476);	
}


//
// DHCP handling. Get the initial IP
uint16_t Get_DHCP_Config(void){
	uint8_t rval= 0;
	uint16_t plen;
	//write mac address in the ethernet controller
	init_mac(mymac);
	
while(rval==0){
	plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
	buf[BUFFER_SIZE]='\0';
	rval=packetloop_dhcp_initial_ip_assignment(buf,plen,mymac[5]);
	}
	
	// we have an IP:
	dhcp_get_my_ip(myip,netmask,gwip);
	client_ifconfig(myip,netmask);
#if 0 //create error notifie through LEDS	
	if (gwip[0]==0){
		// we must have a gateway returned from the dhcp server
		// otherwise this code will not work
	//	PD0LEDON; // error
		while(1); // stop here
	}
#endif	

	// we have a gateway.
	// find the mac address of the gateway (e.g your dsl router).
	get_mac_with_arp(gwip,TRANS_NUM_GWMAC,&arpresolver_result_callback);
		
	return plen;
}
