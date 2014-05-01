/*
 * m328IpComm.c
 *
 * Created: 4/30/2014 10:02:49 PM
 *  Author: John
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "m328IpComm.h"
#include "../drivers/drivers.h"

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>


int main(void)
{
	
	uint16_t dat_p,plen;
	uint8_t payloadlen=0;
	char str[20];
	uint8_t rval;
	uint16_t i=0;
	uint8_t fd;
				
	//Init specific controller peripherals
	MCU_Init();
	

	plen = Get_DHCP_Config();
//	while(1){
	//	USART_Transmit(0x31);
//	}
	
	while(get_mac_with_arp_wait()){
		// to process the ARP reply we must call the packetloop
		plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
		packetloop_arp_icmp_tcp(buf,plen);
	}

#if 0 //ramane daca vrem dns
	if (string_is_ipv4(WEBSERVER_VHOST)){
		// the the webserver_vhost is not a domain name but alreay
		// an IP address written as sting
		parse_ip(otherside_www_ip,WEBSERVER_VHOST);
		processing_state=2; // no need to do any dns look-up
	}
#endif
		
    while(1)
    {
		USART_Transmit(0x32);
		plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
		buf[BUFFER_SIZE]='\0'; // http is an ascii protocol. Make sure we have a string terminator.
		// DHCP renew IP:
		plen=packetloop_dhcp_renewhandler(buf,plen); // for this to work you have to call dhcp_6sec_tick() every 6 sec
		dat_p=packetloop_arp_icmp_tcp(buf,plen);

#if 1
//	if(plen==0){
{
	//we are idle so send message
	if(i>60000){
		//send_tcp_data();
		fd=client_tcp_req(&your_client_tcp_result_callback,&your_client_tcp_datafill_callback,MYTCPPORT,otherside_www_ip,gwmac);
		i=0;
		continue;
	}else
	i++;
	//continue;
}
#endif


		if(dat_p==0){ // plen!=0
			// check for incomming messages not processed
			// as part of packetloop_arp_icmp_tcp, e.g udp messages
			//udp_client_check_for_dns_answer(buf,plen);
			//continue;
			// check for udp
			goto UDP;
		}
		
UDP:
		// check if ip packets are for us:
		if(eth_type_is_ip_and_my_ip(buf,plen)==0){
			continue;
		}

		if (buf[IP_PROTO_P] == IP_PROTO_UDP_V &&\
		buf[UDP_DST_PORT_H_P] == (MYUDPPORT>>8) &&\
		buf[UDP_DST_PORT_L_P] == (MYUDPPORT&0xff)) {
			payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
			// you must sent a string starting with v
	
			strcpy(str,"Sensolight! usage: ver");
			make_udp_reply_from_request(buf,str,strnlen(str,35),MYUDPPORT);
		}				
		
		
		
    }
	
	return (0);
}

