/*
 * webpage_help.h
 *
 * Created: 5/6/2014 1:03:44 PM
 *  Author: ionut.alexa
 */ 


#ifndef WEBPAGE_HELP_H_
#define WEBPAGE_HELP_H_

extern const char index_html[];
extern const char config_html[];

// Print s text as a string
#define xstr(s)		str(s)
#define str(s)		#s

uint16_t add_values_to_buf(char * buf,uint16_t pos, const char * page, ...);
void print_page(void);


#endif /* WEBPAGE_HELP_H_ */