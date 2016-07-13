#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024
// Server (ordi infecté)

int main( int argc, char** argv )
{
    if( argc != 2 ) {
        fprintf(stderr, "Usage: %s <port>", argv[0]);
        exit(1);
    }
    int udpSocket, nBytes;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    int i, portNum = atoi(argv[1]);

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

    while( 1 )
    {
        /* Try to receive any incoming UDP datagram. Address and port of
          requesting client will be stored on serverStorage variable */
        nBytes = recvfrom( udpSocket, buffer, BUF_SIZE, 0, (struct sockaddr *) &serverStorage, &addr_size );

        /* Send uppercase message back to client, using serverStorage as the address */
        sendto( udpSocket, "OK", nBytes, 0, (struct sockaddr *) &serverStorage, addr_size );
    }

    return 0;
}
