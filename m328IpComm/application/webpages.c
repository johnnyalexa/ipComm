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
*	<div>  .... </div>
***************************************************/
const char main_div[] PROGMEM =QUOTE(
<div style="width:500px">
);

const char end_div[] PROGMEM =QUOTE(
</div>
);

/**************************************************
*	<div>  Header  </div>
***************************************************/
const char hdr_div[] PROGMEM =QUOTE(
<div style="background-color:#FFA500;">
<h1 style="margin-bottom:0;">IpComm Ethernet Communicator</h1></div>
);

/**************************************************
*	<div>  Menu </div>
***************************************************/
const char menu_div[] PROGMEM =QUOTE(
<div style="background-color:#FFD700;height:185px;width:150px;float:left;">
<b>Menu</b><br>
<a href=/c>[config]</a><br>
<a href=./>[refresh]</a><br><br><br>
<img src="http://www.freshcreative.ro/portofoliu/slf_small.jpg"width="150" height="75"/>
</div>
);

/**************************************************
*	<div>  content </div>
***************************************************/
const char content_div[] PROGMEM =QUOTE(
<div id="content" style="background-color:#EEEEEE;height:185px;width:350px;float:left;">
<center><b>Current config:</b></center>
);

/**************************************************
*	<div>  footer </div>
***************************************************/
const char footer_div[] PROGMEM =QUOTE(
<div id="ft" style="background-color:#FFA500;clear:both;text-align:center;">
Copyright www.slf.ro</div>
);

/**************************************************
*	<div> table </div>
***************************************************/
const char table_start[] PROGMEM =QUOTE(
<table border="1" style="width:350px">
);
const char table_end[] PROGMEM =QUOTE(
</table>
);

/**************************************************
*	<div> Table line: string1 | string2 </div>
***************************************************/
const char table_line_MAC[] PROGMEM =QUOTE(
<tr>
<td>%s</td>
<td>%02X:%02X:%02X:%02X:%02X:%02X</td>
</tr>
);

const char table_line_IP[] PROGMEM =QUOTE(
<tr>
<td>%s</td>
<td>%d.%d.%d.%d</td>
</tr>
);

const char table_line_Port[] PROGMEM =QUOTE(
<tr>
<td>%s</td>
<td>%u</td>
</tr>
);
/**************************************************
*	Index.html 
***************************************************/
const char index_html[] PROGMEM =QUOTE(
<a href=/c>[config]</a>
<a href=./>[refresh]</a>
<h2>Alarm2: %d\n
</h2><pre>\n
Last alarm:\n
%d hours and %d min ago\n
</pre><hr>\n
);

#if 0
"\
<a href=/c>[config]</a>\
<a href=./>[refresh]</a>\
<h2>Alarm: %d\n\
</h2><pre>\n\
Last alarm:\n\
%d hours and %d min ago\n\
</pre><hr>\n";
#endif

/**************************************************
*	Config.html
***************************************************/
const char config_html[] PROGMEM =QUOTE(
<a href=/>[home]</a>
<h2>Alarm config:</h2>
);


