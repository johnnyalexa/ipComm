extern int
sslc_init(char* serverIpAddress, unsigned short serverPort, int portType, int *mySocket, const char *pCn)
{
    sbyte4   connectionInstance = 0;
    sbyte4   s4QueueId = 0;
    ipaddr_t host;
    int      status = SSLC_OK;
    sslConnect *ssl = NULL;
    
    /* Initial TCP Queue */
    s4QueueId = sslc_find_a_queue();

    if (s4QueueId == -1)
    {
       status = -1;
       goto exit;
    }
    DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)"Connecting to host ");
    DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)serverIpAddress);
    DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)":");
    DEBUG_INT(DEBUG_SSL_MESSAGES, serverPort);
    DEBUG_PRINTNL(DEBUG_SSL_MESSAGES, (sbyte *)"");

    if (SSLC_OK > (status = sslc_create_connect(serverIpAddress, serverPort, mySocket, &host, portType))) {
        DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)"host addr type ");
        DEBUG_INT(DEBUG_SSL_MESSAGES, host.type);
        DEBUG_PRINTNL(DEBUG_SSL_MESSAGES, (sbyte *)"");

        sslc_release_a_queue(s4QueueId);
        goto exit;
    }

    DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)"Passed sslc_create_connect ");
    DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)serverIpAddress);
    DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)" socket ");
    DEBUG_HEXINT(DEBUG_SSL_MESSAGES, *mySocket);
    DEBUG_PRINTNL(DEBUG_SSL_MESSAGES, (sbyte *)"");
    ssl = (sslConnect *)*mySocket;
    ssl->s4QueueId = s4QueueId;
    
    if (OK > (connectionInstance = SSL_connect((sbyte4)ssl->tcp, 0, NULL, NULL, (sbyte *)pCn)))
    {
        DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)"SSL_connect error ");
        DEBUG_INT(DEBUG_SSL_MESSAGES, connectionInstance);
        DEBUG_PRINTNL(DEBUG_SSL_MESSAGES, (sbyte *)"");

        status = connectionInstance;
        sslc_release_a_queue(s4QueueId);
        sslc_glue_tcp_close((struct tcb *)ssl->tcp); 
        GLUE_FREE(ssl);
        *mySocket = NULL; 
        goto exit;
    }

    DEBUG_PRINTNL(DEBUG_SSL_MESSAGES, (sbyte *)"passed SSL_connect");
    if (OK > (status = SSL_assignCertificateStore(connectionInstance, host_cert_store)))
    {
        DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)"SSL_assignCertificiateStore error = ");
        DEBUG_INT(DEBUG_SSL_MESSAGES, status);
        DEBUG_PRINTNL(DEBUG_SSL_MESSAGES, (sbyte *)"");
        sslc_release_a_queue(s4QueueId);
        sslc_glue_tcp_close((struct tcb *)ssl->tcp); 
        SSL_closeConnection(connectionInstance);
        GLUE_FREE(ssl);
        *mySocket = NULL; 
        goto exit;
    }

    if (OK > (status = SSL_negotiateConnection(connectionInstance)))
    {
        DEBUG_PRINT(DEBUG_SSL_MESSAGES, (sbyte *)"SSL_negotiateConnection error = ");
        DEBUG_INT(DEBUG_SSL_MESSAGES, status);
        DEBUG_PRINTNL(DEBUG_SSL_MESSAGES, (sbyte *)"");
        sslc_release_a_queue(s4QueueId);
        sslc_glue_tcp_close((struct tcb *)ssl->tcp); 
        SSL_closeConnection(connectionInstance);
        GLUE_FREE(ssl);
        *mySocket = NULL; 
        goto exit;
    }

    (ssl->tcp)->sslConnectionInstance = connectionInstance;
    ssl->instance = connectionInstance;
    
exit:
    return status;
}