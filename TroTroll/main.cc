/*
 * C++
 * main.cc
 *
 * 15 Juillet 2016
 *
 * Server (ordi infecté)
 *
 */
/* C++ headers */
#include <iostream>
/* C headers */
#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
/* Defines */
#define BUF_SIZE 1024
/* Internal headers */

int main( int argc, char** argv )
{
    if( argc != 2 ) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    int udpSocket, nBytes;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr/*, clientAddr*/;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    int portNum = atoi(argv[1]);

    /* Create UDP socket */
    udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    /* Configure settings in address struct */
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /* Bind socket with address struct */
    bind( udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr) );

    /* Initialize size variable to be used later on */
    addr_size = sizeof serverStorage;
    memset( buffer, '\0', BUF_SIZE );

    while( 1 )
    {
        printf("Waiting for data...\n");
        fflush(stdout);
        /* Try to receive any incoming UDP datagram. Address and port of
          requesting client will be stored on serverStorage variable */
        nBytes = recvfrom( udpSocket, buffer, BUF_SIZE, 0, (struct sockaddr *) &serverStorage, &addr_size );
        assert( nBytes != -1 );
        printf("Received packet from %s:%d (%d bytes)\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port), nBytes);
        printf("Data: %s\n" , buffer);
        memset( buffer, '\0', BUF_SIZE );

        /* Send to client, using serverStorage as the address */
        sendto( udpSocket, "OK", BUF_SIZE, 0, (struct sockaddr *) &serverStorage, addr_size );
    }

    return 0;
}
