/*
 * webserver1.c
 *
 * Created: 5/5/2014 9:13:00 PM
 *  Author: John
 */ 
#include "ip_config.h"
#include "m328IpComm.h"
#include "webpage_help.h"
#include "../enc28j60_tcp_ip_stack/include/ip_arp_udp_tcp.h"
#include "../enc28j60_tcp_ip_stack/include/websrv_help_functions.h"
#include "../enc28j60_tcp_ip_stack/include/net.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>



#define STR_BUFFER_SIZE 30
#define MYNAME_LEN STR_BUFFER_SIZE-14

static uint16_t gPlen;
static char gStrbuf[STR_BUFFER_SIZE+1];
//static char buf2[100];
//static uint8_t alarmOn=1; // alarm system is on or off
//static uint8_t lastAlarm=0; // indicates if we had an alarm or not

// the password string (only a-z,0-9,_ characters):
static char password[10]="sharedsec"; // must not be longer than 9 char



//"<pre>Hi!\nYour web server works great.\nMy Ip: %d.%d.%d.%d\n</pre>\n";


const char page1[] PROGMEM = "<pre>Hi!\nYour web server works great.\nMy Ip: %d.%d.%d.%d\n</pre>\n";



uint8_t verify_password(char *str)
{
	// the first characters of the received string are
	// a simple password/cookie:
	if (strncmp(password,str,strlen(password))==0){
		return(1);
	}
	return(0);
}

uint16_t http200ok(uint8_t * buf)
{
	return(fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n")));
}

#if 0 //unused
uint16_t print_webpage_config(uint8_t * buf)
{
	uint16_t plenc=0;
	char buf2[200];
	char buf3[200];
		
	plenc=http200ok(buf);
	
	plenc=fill_tcp_data_p(buf,plenc,form_post);   // Start form
	plenc=fill_tcp_data_p(buf,plenc,main_div);    //Main div of the page         -----------------------------------
	
	/* Header </div> terminated */
	strcpy_P(buf2,hdr_div);						                             //  -----------------------------------
	sprintf(buf3, buf2,"SLF IpComm Configuration");                          //  *  SLF IpComm Configuration       *
	plenc=fill_tcp_data(buf,plenc,buf3);                                     //  ----------------------------------- 
	
	
	/* Start the table*/                                                    //   ___________________________________
	plenc=fill_tcp_data_p(buf,plenc,table_start);                           //   |                                 |
	
//____________________________________________________________________________________________________________________//	
	plenc=fill_tcp_data_p(buf,plenc,table_tr_start);						//	<tr>
		plenc=fill_tcp_data_p(buf,plenc,table_line_td_start);				//	<td>
			strcpy_P(buf2,table_line_string);
			sprintf(buf3, buf2,"MAC Address	");								// | Mac Address |	
			plenc=fill_tcp_data(buf,plenc,buf3);
		plenc=fill_tcp_data_p(buf,plenc,table_line_td_stop);				//	</td>
//____________________________________________________________________________________________________________________//	
		plenc=fill_tcp_data_p(buf,plenc,table_line_td_start);				//	<td>
			strcpy_P(buf2,table_line_mac_cfg_start);
			sprintf(buf3, buf2,mymac[0],mymac[1],mymac[2],mymac[3]);		// |02:54:23:43 |
			plenc=fill_tcp_data(buf,plenc,buf3);
			strcpy_P(buf2,table_input_hex_cfg);
			sprintf(buf3, buf2,"mac4","mac4",mymac[4]);		                    // |:01 |
			plenc=fill_tcp_data(buf,plenc,buf3);
			strcpy_P(buf2,table_input_hex_cfg);
			sprintf(buf3, buf2,"mac5","mac5",mymac[5]);		                    // |:02 |
			plenc=fill_tcp_data(buf,plenc,buf3);
		plenc=fill_tcp_data_p(buf,plenc,table_line_td_stop);				//	</td>
	plenc=fill_tcp_data_p(buf,plenc,table_tr_stop);							//	</tr>
//____________________________________________________________________________________________________________________//	
//printf("plenc=%d",plenc);
	plenc=fill_tcp_data_p(buf,plenc,table_tr_start);						//	<tr>
		plenc=fill_tcp_data_p(buf,plenc,table_line_td_start);				//	<td>
			strcpy_P(buf2,table_line_string);
			sprintf(buf3, buf2,"Server IP");								// | Server IP |
			plenc=fill_tcp_data(buf,plenc,buf3);
		plenc=fill_tcp_data_p(buf,plenc,table_line_td_stop);				//	</td>
//____________________________________________________________________________________________________________________//
		plenc=fill_tcp_data_p(buf,plenc,table_line_td_start);				//	<td>		
			strcpy_P(buf2,table_input_dec_cfg);
			sprintf(buf3, buf2,"ip0","ip0",otherside_www_ip[0]);		    // |192. |
			plenc=fill_tcp_data(buf,plenc,buf3);
				plenc=fill_tcp_data(buf,plenc,".");
			strcpy_P(buf2,table_input_dec_cfg);
			sprintf(buf3, buf2,"ip1","ip1",otherside_www_ip[1]);		    // |168. |
			plenc=fill_tcp_data(buf,plenc,buf3);
				plenc=fill_tcp_data(buf,plenc,".");
			strcpy_P(buf2,table_input_dec_cfg);
			sprintf(buf3, buf2,"ip2","ip2",otherside_www_ip[2]);		    // |0. |
			plenc=fill_tcp_data(buf,plenc,buf3);
				plenc=fill_tcp_data(buf,plenc,".");
			strcpy_P(buf2,table_input_dec_cfg);
			sprintf(buf3, buf2,"ip3","ip3",otherside_www_ip[3]);		    // |111 |
			plenc=fill_tcp_data(buf,plenc,buf3);								
		
		plenc=fill_tcp_data_p(buf,plenc,table_line_td_stop);				//	</td>		
	plenc=fill_tcp_data_p(buf,plenc,table_tr_stop);							//	</tr>

	/*
	//printf("plen=%d\n",plenc);
	strcpy_P(buf2,table_line_ip_cfg);
	sprintf(buf3, buf2,
			otherside_www_ip[0],
			otherside_www_ip[1],
			otherside_www_ip[2]);
	plenc=fill_tcp_data(buf,plenc,buf3); */
	//printf("plen=%d\n",plenc);
	
/*	strcpy_P(buf2,table_line_port_cfg);						
	sprintf(buf3, buf2,167);
	plenc=fill_tcp_data(buf,plenc,buf3);
	*/	

	
	plenc=fill_tcp_data_p(buf,plenc,table_end);
	

	
	plenc=fill_tcp_data_p(buf,plenc,footer_div); 
	plenc=fill_tcp_data_p(buf,plenc,end_div);	 //div1
	plenc=fill_tcp_data_p(buf,plenc,end_form);   
	

SYS_LOG("plen=%d\n",plenc);
	return(plenc);
}

uint16_t print_webpage_login(uint8_t * buf)
{
	uint16_t plenl=0;
	char buf2[200];
	char buf3[200];
	
	plenl=http200ok(buf);
	
	plenl=fill_tcp_data_p(buf,plenl,form_post);
	plenl=fill_tcp_data_p(buf,plenl,main_div);    //div1
	
	
	strcpy_P(buf2,hdr_div);						/* </div> terminated */
	sprintf(buf3, buf2,"SLF IpComm Login");
	plenl=fill_tcp_data(buf,plenl,buf3);
	
	plenl=fill_tcp_data_p(buf,plenl,login_form);
	plenl=fill_tcp_data_p(buf,plenl,submit_reset_button);
	
	plenl=fill_tcp_data_p(buf,plenl,footer_div);
	plenl=fill_tcp_data_p(buf,plenl,end_div);	 //div1
	plenl=fill_tcp_data_p(buf,plenl,end_form);
	
	return plenl;	
}
#endif

										  
// main web page
uint16_t print_webpage(uint8_t * buf)
{
	uint16_t plen=0;
	char buf2[150];
	char buf3[150];
	static int srv_port;
	srv_port = currentConfig.server_port;
	
	plen=http200ok(buf);
	
	plen=fill_tcp_data_p(buf,plen,main_div);				//div1
	
	strcpy_P(buf2,hdr_div);									// </div> terminated 
	sprintf(buf3, buf2,"IpComm Ethernet Communicator");
	plen=fill_tcp_data(buf,plen,buf3);
	
	plen=fill_tcp_data_p(buf,plen,content_div); //div2
	
	// --------------  TABLE START ------------------ //	
	plen=fill_tcp_data_p(buf,plen,table_start);
	// --------------  MAC ADDRESS ------------------ //
	plen=fill_tcp_data_p(buf,plen,table_tr_start);			// <tr>	||
	strcpy_P(buf2,table_line_string);						// | string | 
	sprintf(buf3, buf2,"MAC Address");
	plen=fill_tcp_data(buf,plen,buf3);
	strcpy_P(buf2,table_line_MAC);
	sprintf(buf3, buf2,	mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);
	plen=fill_tcp_data(buf,plen,buf3);
	plen=fill_tcp_data_p(buf,plen,table_tr_stop);			// </tr>	||
	// --------------  IP ADDRESS  ------------------ //
	plen=fill_tcp_data_p(buf,plen,table_tr_start);			// <tr>	||
	strcpy_P(buf2,table_line_string);						// | string |
	sprintf(buf3, buf2,"IP Address");
	plen=fill_tcp_data(buf,plen,buf3);
	strcpy_P(buf2,table_line_IP);
	sprintf(buf3, buf2,myip[0],myip[1],myip[2],myip[3]);
	plen=fill_tcp_data(buf,plen,buf3);
	plen=fill_tcp_data_p(buf,plen,table_tr_stop);			// </tr>	||
	// --------------    NETMASK   ------------------ //	
	plen=fill_tcp_data_p(buf,plen,table_tr_start);			// <tr>	||
	strcpy_P(buf2,table_line_string);						// | string |
	sprintf(buf3, buf2,"Netmask");
	plen=fill_tcp_data(buf,plen,buf3);
	strcpy_P(buf2,table_line_IP);
	sprintf(buf3, buf2,netmask[0],netmask[1],netmask[2],netmask[3]);
	plen=fill_tcp_data(buf,plen,buf3);
	plen=fill_tcp_data_p(buf,plen,table_tr_stop);			// </tr>	||		
	// --------------  GW IP ADDRESS  ---------------- //
	plen=fill_tcp_data_p(buf,plen,table_tr_start);			// <tr>	||
	strcpy_P(buf2,table_line_string);						// | string |
	sprintf(buf3, buf2,"Gateway");
	plen=fill_tcp_data(buf,plen,buf3);
	strcpy_P(buf2,table_line_IP);
	sprintf(buf3, buf2,gwip[0],gwip[1],gwip[2],gwip[3]);
	plen=fill_tcp_data(buf,plen,buf3);
	plen=fill_tcp_data_p(buf,plen,table_tr_stop);			// </tr>	||		
	// ------------  Server IP ADDRESS  --------------- //
	plen=fill_tcp_data_p(buf,plen,table_tr_start);			// <tr>	||
	strcpy_P(buf2,table_line_string);						// | string |
	sprintf(buf3, buf2,"Server IP");
	plen=fill_tcp_data(buf,plen,buf3);
	strcpy_P(buf2,table_line_IP);
	sprintf(buf3, buf2,server_ip[0],server_ip[1],server_ip[2],server_ip[3]);
	plen=fill_tcp_data(buf,plen,buf3);
	plen=fill_tcp_data_p(buf,plen,table_tr_stop);			// </tr>	||		
	// ---------------  Server Port  ----------------- //
	plen=fill_tcp_data_p(buf,plen,table_tr_start);			// <tr>	||
	strcpy_P(buf2,table_line_string);						// | string |
	sprintf(buf3, buf2,"Server Port");
	plen=fill_tcp_data(buf,plen,buf3);
	strcpy_P(buf2,table_line_Port);
	//sprintf(buf3, buf2, MYTCPPORT);
	sprintf(buf3, buf2, srv_port);
	plen=fill_tcp_data(buf,plen,buf3);
	plen=fill_tcp_data_p(buf,plen,table_tr_stop);			// </tr>	||
	// ---------------  TABLE END  ------------------- //	
	plen=fill_tcp_data_p(buf,plen,table_end); 
	
	plen=fill_tcp_data_p(buf,plen,end_div);  //div2
	plen=fill_tcp_data_p(buf,plen,footer_div); 
	plen=fill_tcp_data_p(buf,plen,end_div);  //div1
	
	return(plen);
}


// analyse the url given
// The string passed to this function will look like this:
// ?s=1 HTTP/1.....
// We start after the first slash ("/" already removed)
int8_t analyse_get_url(uint8_t * buf, char *str)
{
	// the first slash:
	if (*str == 'c'){
		// configpage:
	//	gPlen=print_webpage_config(buf);
		return(5);
	}
	
	if (*str == 'i'){
		return (10);
	}
	if (*str == 'u'){
		if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"pw")){
			urldecode(gStrbuf);
			if (verify_password(gStrbuf)){
				if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"n")){
					urldecode(gStrbuf);
					gStrbuf[MYNAME_LEN]='\0';
				//	strcpy(myname,gStrbuf);
				}
				if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"ae")){
				//	alarmOn=1;
					}else{
				//	alarmOn=0;
				}
				/*if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"di")){
					urldecode(gStrbuf);
					if (parse_ip(udpsrvip,gStrbuf)!=0){
						return(-2);
					}
				} */
				/*if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"dp")){
					gStrbuf[4]='\0';
					udpsrvport=atoi(gStrbuf);
				} */
				/*if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"gi")){
					urldecode(gStrbuf);
					if (parse_ip(gwip,gStrbuf)!=0){
						return(-2);
					}
				} */
				//data2eeprom();
				gPlen=http200ok(buf);
				gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<a href=/>[home]</a>"));
				gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h2>OK</h2>"));
				return(10);
			}
		}
		return(-1);
	}
	return(0);
}


// analyse the url given
// The string passed to this function will look like this:
// ?s=1 HTTP/1.....
// We start after the first slash ("/" already removed)
int8_t analyse_post_url(uint8_t * buf, char *str)
{
	// the first slash:
	if (*str == 'c'){
		// configpage:
	//	gPlen=print_webpage_config(buf);
		return(10);
	}
	
	if (*str == 'i'){
		return (5);
	}
	if (*str == 'u'){
		if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"pw")){
			urldecode(gStrbuf);
			if (verify_password(gStrbuf)){
				if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"n")){
					urldecode(gStrbuf);
					gStrbuf[MYNAME_LEN]='\0';
				//	strcpy(myname,gStrbuf);
				}
				if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"ae")){
				//	alarmOn=1;
					}else{
				//	alarmOn=0;
				}
				/*if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"di")){
					urldecode(gStrbuf);
					if (parse_ip(udpsrvip,gStrbuf)!=0){
						return(-2);
					}
				} */
				/*if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"dp")){
					gStrbuf[4]='\0';
					udpsrvport=atoi(gStrbuf);
				} */
				/*if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"gi")){
					urldecode(gStrbuf);
					if (parse_ip(gwip,gStrbuf)!=0){
						return(-2);
					}
				} */
				//data2eeprom();
				gPlen=http200ok(buf);
				gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<a href=/>[home]</a>"));
				gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h2>OK</h2>"));
				return(10);
			}
		}
		return(-1);
	}
	return(0);
}
