// Nguyen Hai Duong
// simple client

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <stdbool.h>    // for bool type

#define BUFFSIZE 256
#define PORT 8888

void error(const char *msg){
    perror(msg);
    exit(0);
}

char *temptTank = (char *)   "{\"data\":\n"
                                "{\n"
                                    "\"temptTank\":{\n"
                                        "\"tempt\":25\n"
                                    "}\n"
                                "}\n"
                            "}";


int main(int argc, char *argv[]){

    int servSock, recvMsgSize;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    char buffer[BUFFSIZE];
    struct sockaddr_in serv_addr;

    /*Create UDP socket*/
    servSock = socket(PF_INET, SOCK_DGRAM, 0);

    // Construct local address structure
    memset(&serv_addr, 0, sizeof(serv_addr));        /* Zero out structure */
    serv_addr.sin_family = AF_INET;                 /* Internet address family */
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    serv_addr.sin_port = htons(PORT);               /* Local port */

    /*Bind socket with address struct*/
    bind(servSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverStorage;
    while(1){
    /* Try to receive any incoming UDP datagram. Address and port of 
      requesting client will be stored on serverStorage variable */
    recvMsgSize = recvfrom(servSock, buffer, BUFFSIZE, 0,(struct sockaddr *)&serverStorage, &addr_size);
    printf("Got message from root: %s\n", buffer);
    if(strcmp(buffer,"data")==0) {
        sendto(servSock, temptTank, strlen(temptTank), 0, (struct sockaddr *)&serverStorage, addr_size);
    }
    bzero(buffer,strlen(buffer));
  }

    close(servSock);
    return 0;
}
