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

//Main application
int main(void)
{
	uint16_t dat_p,plen;
	int8_t cmd;
	static uint16_t gPlen;
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
	
	
	if(run_type)	
		SYS_LOG("cfg\n");
		else
			SYS_LOG("run\n");
	
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
	//init the ethernet/ip layer:
	init_udp_or_www_server(currentConfig.local_mac,myip);
	www_server_port(MYWWWPORT);
			
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
		
		

#if 0
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


		if(dat_p==0){
			//udp_client_check_for_dns_answer(buf,plen);
			//continue; //go to while 1
			goto UDP;
		}
		
		
		
        if ((strncmp("GET ",(char *)&(buf[dat_p]),4)!=0)&&(strncmp("POST",(char *)&(buf[dat_p]),4)!=0)){
          // head, post and other methods: (not GET)
          //
          // for possible status codes see:
          // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
          gPlen=http200ok(buf);
          gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h1>200 OK</h1>"));
          goto SENDTCP;
          }else
			SYS_LOG("dat_p=%d\n",dat_p);
			 //SYS_LOG("buf[%d]=%s\n",dat_p,(char *)&(buf[dat_p]));
		  
		// Cut the size for security reasons. If we are almost at the
		// end of the buffer then there is a zero but normally there is
		// a lot of room and we can cut down the processing time as
		// correct URLs should be short in our case. If dat_p is already
		// close to the end then the buffer is terminated already.
		if ((dat_p+100) < BUFFER_SIZE){
			buf[dat_p+100]='\0'; //100
		}
		
		// start after the first slash:
		cmd=analyse_get_url(buf,(char *)&(buf[dat_p+5]));
        
		if (cmd==-1){
			gPlen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 UnauthorizedB</h1>"));
	        goto SENDTCP;
            }
        if (cmd==-2){
	        gPlen=http200ok(buf);
	        gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h1>ERROR in IP or port number</h1>"));
	        goto SENDTCP;
            }
		if(cmd == 5){
			
			//gPlen=print_webpage_login(buf);
			goto SENDTCP;
		}
        if (cmd==10){
	       // gPlen is already set
		 //  gPlen=print_webpage_config(buf);
	        goto SENDTCP;
            }
 
           // the main page: the login page
		   update_info();
		   gPlen = print_webpage(buf);
		   SYS_LOG("gplen=%d\n",gPlen);
		 //  gPlen = print_webpage_login(buf);
        //   gPlen=print_webpage(buf);
														
SENDTCP:
	www_server_reply(buf,gPlen); // send data
	continue;					
		
		
#if 1		
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
		char str[40];	
		int scanf_rc;
		if (buf[UDP_DATA_P]=='*' ){
			scanf_rc = sscanf(&buf[UDP_DATA_P],
									"*%u.%u.%u.%u:%u",
									&ip[0],
									&ip[1],
									&ip[2],
									&ip[3],
									&port
									);
			
			//	USART_Transmit(0x30+scanf_rc);
			if(scanf_rc !=5){
				strcpy(str,"Config error: Please use format *IP:port");	
			}else{
				uint16_t cfg_rc = ((ip[0]&0xFF00)|(ip[1]&0xFF00)|(ip[2]&0xFF00)|(ip[3]&0xFF00)|(!port));
				if(cfg_rc!=0)
					strcpy(str,"Config error: Incorrect IP or port");
				else
					strcpy(str,"Config OK!");
			}
		}
			
			//int payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
			// you must sent a string starting with v
			//char str[20];
			//SYS_LOG("Enter your response here:\n");
			//scanf("%s",str);
		//	printf("%s",str);
			//strcpy(str,"Sensolight! usage: ver");
			make_udp_reply_from_request(buf,str,strnlen(str,40),MYUDPPORT);
			
		//	strcpy(str,&buf[UDP_DATA_P]);
		//	USART_print(str);
		}		
		
#endif				
		
    } // End of main loop
}else{
//Normal run tpe	
SYS_LOG("else\n");	
}
	SYS_LOG("Reset AVR\n");
// If we get here, we need to restart
Reset_AVR();	
	
	return (0);
}

