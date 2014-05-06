/*
 * ip_config.h
 *
 * Created: 4/29/2014 9:06:37 PM
 *  Author: John
 */ 


#ifndef IP_CONFIG_H_
#define IP_CONFIG_H_

//------------- functions in ip_arp_udp_tcp.c --------------

#define TCP_client


// an NTP client (ntp clock):
#undef NTP_client
// a spontanious sending UDP client (needed as well for DNS and DHCP)
#define UDP_client

// define this if you want to use enc28j60EnableBroadcast/enc28j60DisableBroadcast
// the dhcp_client.c needs this.
#define ENC28J60_BROADCAST

// a server answering to UDP messages
#define UDP_server
//#undef UDP_server
// a web server
#define WWW_server
//#undef WWW_server
//

// to send out a ping:
#undef PING_client
#define PINGPATTERN 0x42

// a UDP wake on lan sender:
#undef WOL_client

// function to send a gratuitous arp
#undef GRATARP

// a "web browser". This can be use to upload data
// to a web server on the internet by encoding the data 
// into the url (like a Form action of type GET):

//#define WWW_client
#undef WWW_client
// if you do not need a browser and just a server:
//#undef WWW_client
//
//------------- functions in websrv_help_functions.c --------------
//
// functions to decode cgi-form data:
//#undef FROMDECODE_websrv_help
#define FROMDECODE_websrv_help

// function to encode a URL (mostly needed for a web client)
#define URLENCODE_websrv_help


#endif /* IP_CONFIG_H_ */

