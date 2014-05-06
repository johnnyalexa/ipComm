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
#include <stdio.h>

#include "m328IpComm.h"
#include "../drivers/drivers.h"
#include "../enc28j60_tcp_ip_stack/include/websrv_help_functions.h"

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>



static ipComm_config_t eep_config;



int main(void)
{
	uint16_t dat_p,plen;
	uint8_t payloadlen=0;
	char str[20];
	uint8_t rval;
	uint16_t i=0;
	uint8_t fd;
	
	int8_t cmd;
	static uint16_t gPlen;
	
				
	//Init specific controller peripherals
	MCU_Init();
	
	plen = Get_DHCP_Config();

	
	while(get_mac_with_arp_wait()){
		// to process the ARP reply we must call the packetloop
		plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
		packetloop_arp_icmp_tcp(buf,plen);
	}


#if 0 //ramane daca vrem dns
	if (string_is_ipv4(WEBSERVER_VHOST)){
		// the the webserver_vhost is not a domain name but already
		// an IP address written as sting
		parse_ip(otherside_www_ip,WEBSERVER_VHOST);
		//processing_state=2; // no need to do any dns look-up
	}
#endif
	
	NVM_GetCurrentPosition();
	NVM_LoadConfig( &eep_config );
	
//	while(1)
	//	printf("loop:%d.%d.%d.%d\n",
		//eep_config.net.local_ip[0],eep_config.net.local_ip[1],
		//eep_config.net.local_ip[2],eep_config.net.local_ip[3]);
	//	printf("size = %d\n",sizeof(ipComm_config_t));
	
	//while(1)
		//printf("My IP=%d.%d.%d.%d\n",otherside_www_ip[0],otherside_www_ip[1],otherside_www_ip[2],otherside_www_ip[3]);
	
	

printf("My IP=%d.%d.%d.%d\n",myip[0],myip[1],myip[2],myip[3]);
	        //init the ethernet/ip layer:
	        init_udp_or_www_server(mymac,myip);
	        www_server_port(MYWWWPORT);
// Main loop of the program		
    while(1)
    {
		//USART_Transmit(0x32);
//		printf("My IP=%d.%d.%d.%d\n",myip[0],myip[1],myip[2],myip[3]);
		plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
		buf[BUFFER_SIZE]='\0'; // http is an ascii protocol. Make sure we have a string terminator.
		// DHCP renew IP:
		plen=packetloop_dhcp_renewhandler(buf,plen); // for this to work you have to call dhcp_6sec_tick() every 6 sec
		dat_p=packetloop_arp_icmp_tcp(buf,plen);
		
		
		if(0 == plen){ //we are idle, process some dns
			
			
			
			
		}
		

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


		if(dat_p==0){ // plen!=0
			// check for incomming messages not processed
			// as part of packetloop_arp_icmp_tcp, e.g udp messages
			//udp_client_check_for_dns_answer(buf,plen);
			continue;
			// check for udp
			//goto UDP;
		}
		
		                if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0){
			                // head, post and other methods:
			                //
			                // for possible status codes see:
			                // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
			                gPlen=http200ok(buf);
			                gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h1>200 OK</h1>"));
			                goto SENDTCP;
		                }
						// Cut the size for security reasons. If we are almost at the
						// end of the buffer then there is a zero but normally there is
						// a lot of room and we can cut down the processing time as
						// correct URLs should be short in our case. If dat_p is already
						// close to the end then the buffer is terminated already.
						if ((dat_p+100) < BUFFER_SIZE){
							buf[dat_p+100]='\0';
						}
	                if (strncmp("/favicon.ico",(char *)&(buf[dat_p+4]),12)==0){
		                // favicon:
		                gPlen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 301 Moved Permanently\r\nLocation: "));
		                gPlen=fill_tcp_data_p(buf,gPlen,PSTR("http://tuxgraphics.org/ico/a.ico"));
		                gPlen=fill_tcp_data_p(buf,gPlen,PSTR("\r\n\r\nContent-Type: text/html\r\n\r\n"));
		                gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h1>301 Moved Permanently</h1>\n"));
		                goto SENDTCP;
	                }
					// start after the first slash:
					cmd=analyse_get_url(buf,(char *)&(buf[dat_p+5]));
                if (cmd==-1){
	                gPlen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
	                goto SENDTCP;
                }
                if (cmd==-2){
	                gPlen=http200ok(buf);
	                gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h1>ERROR in IP or port number</h1>"));
	                goto SENDTCP;
                }
                if (cmd==10){
	                // gPlen is already set
	                goto SENDTCP;
                }
                // the main page:
                gPlen=print_webpage(buf);
														
	SENDTCP:
	www_server_reply(buf,gPlen); // send data
	continue;					
		
		
#if 0		
UDP:
		// check if ip packets are for us:
		if(eth_type_is_ip_and_my_ip(buf,plen)==0){
			//if not, skip the packet
			continue;
		}

		if (buf[IP_PROTO_P] == IP_PROTO_UDP_V &&\
		buf[UDP_DST_PORT_H_P] == (MYUDPPORT>>8) &&\
		buf[UDP_DST_PORT_L_P] == (MYUDPPORT&0xff)) {
			payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
			// you must sent a string starting with v
			
			printf("Enter your response here:\n");
			scanf("%s",str);
		//	printf("%s",str);
			//strcpy(str,"Sensolight! usage: ver");
			make_udp_reply_from_request(buf,str,strnlen(str,35),MYUDPPORT);
			
		//	strcpy(str,&buf[UDP_DATA_P]);
		//	USART_print(str);
		}		
		
#endif				
		
    } // End of main loop
	
// If we get here, we need to restart
Reset_AVR();	
	
	return (0);
}

