/*
 * webpages.c
 *
 * Created: 5/6/2014 10:00:05 PM
 *  Author: John
 */ 
#include <stdio.h>
#include <avr/pgmspace.h>
#include "webpage_help.h"


/**************************************************
*	Index.html 
*
*
*
*
***************************************************/
const char index_html[] PROGMEM ="\
<a href=/c>[config]</a>\
<a href=./>[refresh]</a>\
<h2>Alarm: %d\n\
</h2><pre>\n\
Last alarm:\n\
%d hours and %d min ago\n\
</pre><hr>\n";


/**************************************************
*	Config.html
*
*
*
*
***************************************************/
const char config_html[] PROGMEM ="<a href=/>[home]</a>"
"<h2>Alarm config</h2><pre>\n"
"<form action=/u method=get>"
"Enabled:<input type=checkbox value=1 name=ae ";

