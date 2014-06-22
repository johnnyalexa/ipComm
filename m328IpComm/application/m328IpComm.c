/*
 * m328IpComm.c
 *
 * Created: 4/30/2014 10:02:49 PM
 *  Author: John
 */ 

// Std include files
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Local include files
#include "m328IpComm.h"
#include "../drivers/drivers.h"
#include "../enc28j60_tcp_ip_stack/include/websrv_help_functions.h"

#include <util/delay.h>
#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>


// Current config structure
ipComm_config_t currentConfig;

void update_info(void);
void update_info(void){
	memcpy(mymac,currentConfig.local_mac,sizeof(mymac));
	memcpy(server_ip,currentConfig.server_ip,sizeof(server_ip));
}

void config_main(void);
void default_main(void);
int uart_state_machine(void);
uint16_t comunicator_tcp_datafill_callback(uint8_t fd);

//Main application
int main(void)
{
/*	uint16_t dat_p,plen;
	int8_t cmd;
	static uint16_t gPlen; */
	uint16_t plen;
	int config_rc = 0;
	
	int run_type;					
	//Init specific controller peripherals
	MCU_Init();
	//ms
	SYS_LOG("MCU Reset\n");
	_delay_ms(3000);
	SYS_LOG("MCU Start\n");
	
	run_type = GetResetSw();
	
	//NVM_GetCurrentPosition();
	config_rc = NVM_LoadConfig( &currentConfig );
		
	if(config_rc < 0){
		memcpy(&currentConfig,&defaultConfig,sizeof(ipComm_config_t));
		NVM_SaveConfig(&currentConfig);
		SYS_LOG("Config default\n");
	};
	
/*
	SYS_LOG("Config loaded\n");
	SYS_LOG("My mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
				currentConfig.local_mac[0],
				currentConfig.local_mac[1],
				currentConfig.local_mac[2],
				currentConfig.local_mac[3],
				currentConfig.local_mac[4],
				currentConfig.local_mac[5])	;
				
	SYS_LOG("Server : %d.%d.%d.%d:%d\n",
				currentConfig.server_ip[0],
				currentConfig.server_ip[1],
				currentConfig.server_ip[2],
				currentConfig.server_ip[3],
				currentConfig.server_port);		
				*/	
				
	Ethernet_Init();	
	Ethernet_Leds_Init();		
	plen = Get_DHCP_Config();
	
	while(get_mac_with_arp_wait()){
		// to process the ARP reply we must call the packetloop
		plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
		packetloop_arp_icmp_tcp(buf,plen);
	}
	SYS_LOG("My IP=%d.%d.%d.%d\n",myip[0],myip[1],myip[2],myip[3]);

#if 0 //ramane daca vrem dns
	if (string_is_ipv4(WEBSERVER_VHOST)){
		// the the webserver_vhost is not a domain name but already
		// an IP address written as sting
		parse_ip(otherside_www_ip,WEBSERVER_VHOST);
		//processing_state=2; // no need to do any dns look-up
	}
#endif
	
// config run type
if(run_type !=0){
	config_main();
}else{
//Normal run tpe	
	//config_main(); // temporary
	default_main();
}
	SYS_LOG("Reset AVR\n");
// If we get here, we need to restart
Reset_AVR();	
	
	return (0);
}


void config_main(void){

		uint16_t dat_p,plen;
		//int8_t cmd;
		static uint16_t gPlen;
		
	SYS_LOG("-- Config --\n");
		//init the ethernet/ip layer:
		init_udp_or_www_server(currentConfig.local_mac,myip);
		//www_server_port(MYWWWPORT);
		
		// Main loop of the program
		while(1)
		{
			// Gets a packet from the network receive buffer, if one is available.
			// The packet will by headed by an ethernet header.
			//      maxlen  The maximum acceptable length of a retrieved packet.
			//      packet  Pointer where packet data should be stored.
			// Returns: Packet length in bytes if a packet was retrieved, zero otherwise.
			plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
			// http is an ascii protocol. Make sure we have a string terminator.
			buf[BUFFER_SIZE]='\0';

			// DHCP renew IP: for this to work you have to call dhcp_6sec_tick() every 6 sec
			plen=packetloop_dhcp_renewhandler(buf,plen);
			
			// return 0 to just continue in the packet loop and return the position
			// of the tcp data if there is tcp data part
			dat_p=packetloop_arp_icmp_tcp(buf,plen);

			if(dat_p==0){
				//udp_client_check_for_dns_answer(buf,plen);
				//continue; //go to while 1
				goto UDP;
			}
			
//SENDTCP:
	//www_server_reply(buf,gPlen); // send data
		//continue;
			
UDP:
		// check if ip packets are for us:
	if(eth_type_is_ip_and_my_ip(buf,plen)==0){
	//if not, skip the packet
		continue;
	}

	if (buf[IP_PROTO_P] == IP_PROTO_UDP_V &&\
	buf[UDP_DST_PORT_H_P] == (MYUDPPORT>>8) &&\
	buf[UDP_DST_PORT_L_P] == (MYUDPPORT&0xff)) {
		uint16_t ip[4];
		uint16_t port;
		char str[80];
		char tmpstr[80];
		int scanf_rc;
		int if_reset= 0;
				
		if (strncmp("at+",(char *)&(buf[UDP_DATA_P]),3)==0){
			scanf_rc = sscanf(&buf[UDP_DATA_P],"at+%s",str);
			SYS_LOG("at+\n command=%s",str);	
			
			if(str == strstr(str,"cur")){
				strcpy(tmpstr,"Current server settings: %u.%u.%u.%u:%d");
				sprintf(str, tmpstr,
							currentConfig.server_ip[0],
							currentConfig.server_ip[1],
							currentConfig.server_ip[2],
							currentConfig.server_ip[3],
							currentConfig.server_port);
			}
			
			if(str == strstr(str,"server:")){
				scanf_rc = sscanf(&buf[UDP_DATA_P+9],
							":%u.%u.%u.%u:%u",&ip[0],&ip[1],&ip[2],&ip[3],&port);
			SYS_LOG("scanfrc=%d\n",scanf_rc);
				if(scanf_rc !=5){
					strcpy(str,"Format error. Please use: at+server:10.100.1.23:4600");
					}else{
					uint16_t cfg_rc = ((ip[0]&0xFF00)|(ip[1]&0xFF00)|(ip[2]&0xFF00)|(ip[3]&0xFF00)|(!port));
					if(cfg_rc!=0)
					strcpy(str,"Config error: Incorrect IP or port");
					else{
						strcpy(str,"Config OK!");
						memcpy(currentConfig.server_ip,ip,sizeof(ip));
						currentConfig.server_port=port;
						NVM_SaveConfig(&currentConfig);
						if_reset = 1; //reset avr
					}
				}
			}
			

		}else{
			strcpy(str,"Command not recognized. Please use AT commands!");
		}
		make_udp_reply_from_request(buf,str,strnlen(str,80),MYUDPPORT);
		if(if_reset!=0)
			return;	
	}
			
} // End of main loop
	
}

void default_main(void){
	
	uint16_t dat_p,plen;
	static uint16_t gPlen;
	SYS_LOG("-- Run --\n");
	//init the ethernet/ip layer:
	init_udp_or_www_server(currentConfig.local_mac,myip);
	//www_server_port(MYWWWPORT);
		
	// Main loop of the program
	while(1){
	// Gets a packet from the network receive buffer, if one is available.
	// The packet will by headed by an ethernet header.
	//      maxlen  The maximum acceptable length of a retrieved packet.
	//      packet  Pointer where packet data should be stored.
	// Returns: Packet length in bytes if a packet was retrieved, zero otherwise.
	plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
	// http is an ascii protocol. Make sure we have a string terminator.
	buf[BUFFER_SIZE]='\0';
	// DHCP renew IP: for this to work you have to call dhcp_6sec_tick() every 6 sec
	plen=packetloop_dhcp_renewhandler(buf,plen);
			
	// return 0 to just continue in the packet loop and return the position
	// of the tcp data if there is tcp data part
	dat_p=packetloop_arp_icmp_tcp(buf,plen);

	if(dat_p==0){
		//udp_client_check_for_dns_answer(buf,plen);
		continue; // if upd data, we are not interested
	}
	
	if(uart_state_machine()>0){
		int fd;
		//send_tcp_data();
		fd=client_tcp_req(&your_client_tcp_result_callback,&comunicator_tcp_datafill_callback,
						currentConfig.server_port, currentConfig.server_ip ,gwmac);	
	}else
		continue;

			
SENDTCP:
	www_server_reply(buf,gPlen); // send data
	continue;
	} // End of main loop
}

	static uart_buff[40];

int uart_state_machine(void){
	int rc = -1;
	static uint8_t uart_sm=0;
	static uint8_t uart_pos=0;

	
	if(usart_kbhit()){
		uart_buff[uart_pos]=usart_getchar();
		uart_pos++;
		if(uart_buff[uart_pos-1] == 0x0A)
			rc = uart_pos;
	}
		
	return rc;	
}

//
// Declare a callback function to be called in order to fill in the
// request (tcp data sent to the server):
uint16_t comunicator_tcp_datafill_callback(uint8_t fd){
	return(fill_tcp_data_p(buf,0,uart_buff));
}