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
<div id="content" style="background-color:#EEEEEE;height:200px;width:500px;float:left;">
<center><b> Current config: </b></center>
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
\n
<table border="1" align="center"> //style="width:350px">
);
const char table_end[] PROGMEM =QUOTE(
</table>
);

/**************************************************
*	<div> Table line: string1 | string2 </div>
***************************************************/
const char table_tr_start[] PROGMEM = QUOTE(
<tr>
);
const char table_tr_stop[] PROGMEM = QUOTE(
</tr>
);

const char table_line_string[] PROGMEM = QUOTE(
<td>%s</td>
);

const char table_line_MAC[] PROGMEM =QUOTE(
<td>%02X:%02X:%02X:%02X:%02X:%02X</td>
);

const char table_line_IP[] PROGMEM =QUOTE(
<td>%d.%d.%d.%d</td>
);

const char table_line_Port[] PROGMEM =QUOTE(
<td>%u</td>
);

const char table_line_mac_cfg_start[] PROGMEM =QUOTE(
%02X:%02X:%02X:%02X
);

const char table_input_hex_cfg[] PROGMEM =QUOTE(
:<input NAME="%s" id="%s" TYPE="text" SIZE="1" MAXLENGTH="2" VALUE="%02X">
);

const char table_line_td_start[] PROGMEM =QUOTE(
<td>
);
const char table_line_td_stop[] PROGMEM =QUOTE(
</td>
);

//Can be used to config port or ip
const char table_input_dec_cfg[] PROGMEM =QUOTE(
<input NAME="%s" id="%s" TYPE="text" SIZE="1" MAXLENGTH="3" VALUE="%d">
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


