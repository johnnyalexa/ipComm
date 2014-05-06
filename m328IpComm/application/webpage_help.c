/*
 * webpage_help.c
 *
 * Created: 5/6/2014 1:07:38 PM
 *  Author: ionut.alexa
 */ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <avr/pgmspace.h>
#include "webpage_help.h"



#define PAGE1 \
<pre>Hi!\
Your web server12 wweqewqewq great.\
My Ip: %d.%d.%d.%d\
</pre>\

const char page3[] PROGMEM = xstr(PAGE1);

uint16_t add_values_to_buf(char * buf, const char * page, ...){
	// Create temporary buffer
	char * buf2=malloc(200);
	// Create list of arguments
	va_list args;
	// Load arguments into list
	va_start(args,page);
	//strcpy_P(buf2,page);
	// Complete the formated text with values from arguments
	vsprintf(buf2,page,args);
	// print resulting text
	printf(buf2);
	// End argument list
	va_end(args);
	//sprintf(buf2,buf2,);
	// Free the buffer
	free(buf2);
}



void print_page(void){
	char buf[300];
	add_values_to_buf(buf,page3,1,2,3,4);
}