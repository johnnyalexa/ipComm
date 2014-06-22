/*
 * obsolete.c
 *
 * Created: 6/22/2014 8:23:14 PM
 *  Author: John
 */ 
#if 0  // not used


			
			if ((strncmp("GET ",(char *)&(buf[dat_p]),4)!=0)&&(strncmp("POST",(char *)&(buf[dat_p]),4)!=0)){
				// head, post and other methods: (not GET)
				//
				// for possible status codes see:
				// http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
				gPlen=http200ok(buf);
				gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h1>200 OK</h1>"));
				goto SENDTCP;
			}else
			SYS_LOG("dat_p=%d\n",dat_p);
			//SYS_LOG("buf[%d]=%s\n",dat_p,(char *)&(buf[dat_p]));
			
			// Cut the size for security reasons. If we are almost at the
			// end of the buffer then there is a zero but normally there is
			// a lot of room and we can cut down the processing time as
			// correct URLs should be short in our case. If dat_p is already
			// close to the end then the buffer is terminated already.
			if ((dat_p+100) < BUFFER_SIZE){
				buf[dat_p+100]='\0'; //100
			}
			
			// start after the first slash:
			cmd=analyse_get_url(buf,(char *)&(buf[dat_p+5]));
			
			if (cmd==-1){
				gPlen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 UnauthorizedB</h1>"));
				goto SENDTCP;
			}
			if (cmd==-2){
				gPlen=http200ok(buf);
				gPlen=fill_tcp_data_p(buf,gPlen,PSTR("<h1>ERROR in IP or port number</h1>"));
				goto SENDTCP;
			}
			if(cmd == 5){
				
				//gPlen=print_webpage_login(buf);
				goto SENDTCP;
			}
			if (cmd==10){
				// gPlen is already set
				//  gPlen=print_webpage_config(buf);
				goto SENDTCP;
			}
			
			// the main page: the login page
			update_info();
			gPlen = print_webpage(buf);
			SYS_LOG("gplen=%d\n",gPlen);
			//  gPlen = print_webpage_login(buf);
			//   gPlen=print_webpage(buf);





			#if 0
			//	if(plen==0){
			{
				//we are idle so send message
				if(i>60000){
					//send_tcp_data();
					fd=client_tcp_req(&your_client_tcp_result_callback,&your_client_tcp_datafill_callback,MYTCPPORT,otherside_www_ip,gwmac);
					i=0;
					continue;
				}else
				i++;
				//continue;
			}
			#endif








#endif


