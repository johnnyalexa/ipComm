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
<h1 style="margin-bottom:0;text-align:center;">%s</h1></div>
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
Copyright 2014 SENSOLIGHT - www.slf.ro</div>
);

/**************************************************
*	<div> table </div>
***************************************************/
const char table_start[] PROGMEM =QUOTE(
<br>
<table border="1" align="center"> //style="width:350px">
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

const char table_line_mac_cfg[] PROGMEM =QUOTE(
<tr>
<td>MAC Address</td>
<td>%02X:%02X:%02X:%02X:
<input NAME="mac4" id="mac4" TYPE="text" SIZE="1" MAXLENGTH="2" VALUE="%02X">:
<input NAME="mac5" id="mac5" TYPE="text" SIZE="1" MAXLENGTH="2" VALUE="%02X">
</td>
</tr>  
);

const char table_line_ip_cfg[] PROGMEM =QUOTE(
<tr>
<td>Server IP</td>
<td>
<input NAME="ip0" id="ip0" TYPE="text" SIZE="1" MAXLENGTH="3" VALUE="%d">
<input NAME="ip1" id="ip1" TYPE="text" SIZE="1" MAXLENGTH="3" VALUE="%d">
</td>
</tr>
);

/*
.<input NAME="ip1" id="ip1" TYPE="text" SIZE="1" MAXLENGTH="3" VALUE="%d">
.<input NAME="ip2" id="ip2" TYPE="text" SIZE="1" MAXLENGTH="3" VALUE="%d">
<input NAME="ip3" id="ip3" TYPE="text" SIZE="1" MAXLENGTH="3" VALUE="%d">

*/

const char table_line_port_cfg[] PROGMEM =QUOTE(
<tr>
<td>Server Port</td>
<td>
<input NAME="port" id="port" TYPE="text" SIZE="1" MAXLENGTH="5" VALUE="%d">
</td>
</tr>
);

/**************************************************
*	Form
***************************************************/

const char form_post[] PROGMEM =QUOTE(
<form action="/i" method="POST">
);
const char end_form[] PROGMEM =QUOTE(
</form>
);

/**************************************************
*	Login Form
***************************************************/
const char login_form[] PROGMEM =QUOTE(
<br>
<table border="1" cellspacing="1" cellpadding="0" align="center">
<tr>
<td>&nbsp;Username:&nbsp;</td>
<td>
<input NAME="username" id="username" TYPE="text" MAXLENGTH="256" VALUE="">
</td>
</tr>
<tr>
<td>&nbsp;Password:&nbsp;</td>
<td>
<input NAME="password" id="password" TYPE="password" autocomplete="off" MAXLENGTH="256" VALUE="">
</td>
</tr>
</table>
);

/**************************************************
*	Buttons for reset and Submit
***************************************************/
const char submit_reset_button[] PROGMEM =QUOTE(
<p>
<input TYPE="submit" VALUE="Login" style="margin-left:130px">
<input TYPE="reset" VALUE="Reset" style="margin-left:125px">
</p>
);

/**************************************************
*	Buttons for reset and Submit
***************************************************/
/*
const char submit_reset_button[] PROGMEM =QUOTE(
<p>
<input TYPE="submit" VALUE="Login" style="margin-left:130px">
<input TYPE="reset" VALUE="Reset" style="margin-left:125px">
</p>
);

*/














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


