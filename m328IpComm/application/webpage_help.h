/*
 * webpage_help.h
 *
 * Created: 5/6/2014 1:03:44 PM
 *  Author: ionut.alexa
 */ 


#ifndef WEBPAGE_HELP_H_
#define WEBPAGE_HELP_H_

extern const char main_div[];
extern const char hdr_div[];
extern const char end_div[];
extern const char menu_div[];
extern const char content_div[];
extern const char footer_div[];

extern const char table_start[];
extern const char table_end[];
extern const char table_line_MAC[];
extern const char table_line_IP[];

extern const char index_html[];
extern const char config_html[];

// Print s text as a string
#define QUOTE(...) #__VA_ARGS__
#define xstr(s)		str(s)
#define str(s)		#s

uint16_t add_values_to_buf(char * buf,uint16_t pos, const char * page, ...);
void print_page(void);


#endif /* WEBPAGE_HELP_H_ */