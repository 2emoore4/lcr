/*****************************************************************************
 **             TEXAS INSTRUMENTS PROPRIETARY INFORMATION
 **
 **  (c) Copyright, Texas Instruments Incorporated, 2012-2013.
 **      All Rights Reserved.
 **
 **  Property of Texas Instruments Incorporated. Restricted Rights -
 **  Use, duplication, or disclosure is subject to restrictions set
 **  forth in TI's program license agreement and associated documentation.
 ******************************************************************************/
/**
 *
 * @file    tcp_client.c
 *
 * @brief	TCP implementation using <system supported function > APIs.
 **/
/*****************************************************************************/


/*Include header files related to TCP/IP communication */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#include "tcp_client.h"


int TCP_Connect(char *host, unsigned long int port)
{
/*
    int socket = 0;
    //1. Declare and Initialize TCP/IP module
    //2. Create a socket for connecting to server
    //3. Connect to server.
    return socket;
*/
    struct sockaddr_in   server;
    struct hostent *hptr;
    int sock;
    int ret;

    hptr = gethostbyname(host);

    if(!hptr)
    {
        fprintf(stderr, "Invalid Host Name\n");
        return -1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    memcpy((char *)&server.sin_addr,  hptr->h_addr_list[0], hptr->h_length);
    server.sin_family = AF_INET;
    server.sin_port = htons((short)port);

    ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if(ret < 0)
    {
        fprintf(stderr, "Unable to connect %d\n", ret);
        return -1;
    }

    return sock;	
}

int TCP_Send(int sock, unsigned char *buffer, int length)
{
    // Send 'length' number of bytes from buffer via provided
    // socket <sock> address
	int sent = 0;
	while(sent < length)
	{
		int ret =  send(sock, buffer + sent, length - sent, 0);

		if(ret <= 0)
			return -1;

		sent += ret;
	}

    return 0;
}

int TCP_Receive(int sock, unsigned char *buffer, int length)
{
    //Retrieve 'length' number of bytes into 'buffer' from the socket <sock> address
	int rcvd = 0;

	while(rcvd < length)
	{
		int ret =  recv(sock, buffer + rcvd, length - rcvd, 0);

		if(ret <= 0)
			return -1;

		rcvd += ret;
	}

    return 0;
}

int TCP_Disconnect(int sock)
{
    //free and cleanup socket occupied memory

    return 0;
}
