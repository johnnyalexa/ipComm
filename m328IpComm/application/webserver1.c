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

uint16_t print_webpage_config(uint8_t * buf)
{
	uint16_t plen;
	plen=http200ok(buf);
	plen=fill_tcp_data_p(buf,plen,PSTR("<a href=/>[home]</a>"));
	plen=fill_tcp_data_p(buf,plen,PSTR("<h2>Alarm config</h2><pre>\n"));
	plen=fill_tcp_data_p(buf,plen,PSTR("<form action=/u method=get>"));
	plen=fill_tcp_data_p(buf,plen,PSTR("Enabled:<input type=checkbox value=1 name=ae "));
//	if (alarmOn){
	//	plen=fill_tcp_data_p(buf,plen,PSTR("checked"));
//	}
	plen=fill_tcp_data_p(buf,plen,PSTR(">\nName:   <input type=text name=n value=\""));
	//plen=fill_tcp_data(buf,plen,myname);
	plen=fill_tcp_data_p(buf,plen,PSTR("\">\nSendto: ip=<input type=text name=di value="));
	//mk_net_str(gStrbuf,udpsrvip,4,'.',10);
	//plen=fill_tcp_data(buf,plen,gStrbuf);
	plen=fill_tcp_data_p(buf,plen,PSTR("> port=<input type=text name=dp size=4 value="));
	//itoa(udpsrvport,gStrbuf,10);
	//plen=fill_tcp_data(buf,plen,gStrbuf);
	plen=fill_tcp_data_p(buf,plen,PSTR("> gwip=<input type=text name=gi value="));
//	mk_net_str(gStrbuf,gwip,4,'.',10);
	//plen=fill_tcp_data(buf,plen,gStrbuf);
	plen=fill_tcp_data_p(buf,plen,PSTR(">\nPasswd: <input type=password name=pw>\n<input type=submit value=change></form>\n<hr>"));
	return(plen);
}

#define MSG "Value of Pi = %d.%d blabla11dfadfasfs"
const char page2[] PROGMEM ="Value of Pi = %d.%d blabla";
// main web page
uint16_t print_webpage(uint8_t * buf)
{
	uint16_t plen;
	plen=http200ok(buf);
	
	print_page();
	
	//char buf2[100];
	
	//sprintf(buf2,page1,myip[0],myip[1],myip[2],myip[3]);
	//plen=fill_tcp_data_p(buf,plen,page1);
	//sprintf(gStrbuf,page1);
	//itoa(123%60,gStrbuf,10); // convert integer to string
	//strcpy(gStrbuf,page1);
	
	// sprintf(gStrbuf, "Value of Pi = %d", 314);
	

	//sprintf(buf2, page2, 314,26);
	// plen=fill_tcp_data(buf,0,buf2);
	//plen=fill_tcp_data_p(buf,plen,page1);
	/*
	plen=fill_tcp_data_p(buf,plen,PSTR("<a href=/c>[config]</a> <a href=./>[refresh]</a>"));
	plen=fill_tcp_data_p(buf,plen,PSTR("<h2>Alarm: "));
	//plen=fill_tcp_data(buf,plen,myname);
	plen=fill_tcp_data_p(buf,plen,PSTR("</h2><pre>\n"));
	plen=fill_tcp_data_p(buf,plen,PSTR("Last alarm:\n"));
	if (lastAlarm){
		if (gMin>59){
			itoa(gMin/60,gStrbuf,10); // convert integer to string
			plen=fill_tcp_data(buf,plen,gStrbuf);
			plen=fill_tcp_data_p(buf,plen,PSTR("hours and "));
		}
		itoa(gMin%60,gStrbuf,10); // convert integer to string
		plen=fill_tcp_data(buf,plen,gStrbuf);
		plen=fill_tcp_data_p(buf,plen,PSTR("min ago"));
		}else{
		plen=fill_tcp_data_p(buf,plen,PSTR("none in last 14 days"));
	} */
/*
	plen=fill_tcp_data_p(buf,plen,PSTR("\n</pre><hr>\n"));
	*/
	return(plen);
}

// analyse the url given
//                The string passed to this function will look like this:
//                ?s=1 HTTP/1.....
//                We start after the first slash ("/" already removed)
int8_t analyse_get_url(uint8_t * buf, char *str)
{
	// the first slash:
	if (*str == 'c'){
		// configpage:
		gPlen=print_webpage_config(buf);
		return(10);
	}
	if (*str == 'u'){
		if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"pw")){
			urldecode(gStrbuf);
			if (verify_password(gStrbuf)){
				if (find_key_val(str,gStrbuf,STR_BUFFER_SIZE,"n")){
					urldecode(gStrbuf);
				//	gStrbuf[MYNAME_LEN]='\0';
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
