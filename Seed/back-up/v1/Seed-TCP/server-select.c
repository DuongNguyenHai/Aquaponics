// Nguyen Hai Duong
// Date : 11 jun 2016

#include "TCP-IP.h"
#include "TCP-IP.c"
#include <sys/wait.h>/* for waitpid() */
#include <sys/time.h>       /* for struct timeval {} */
#include <fcntl.h>          /* for fcntl() */

void handle(int clntSock, fd_set *set)
{
    /* send(), recv(), close() */
    /* Call FD_CLR(newsock, set) on disconnection */
    int recvMsgSize;                    /* Size of received message */
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);
    
    // while(1){
        recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
        if (recvMsgSize < 0) 
            perror("ERROR reading from socket");
        else if(recvMsgSize>0){
            printf(". Client[%d]: %s\n",clntSock,buffer);
            bzero(buffer,strlen(buffer));
        }
        else{
            printf("- Client[%d]: disconnected !\n",clntSock);
            // break;
            close(clntSock);
            FD_CLR(clntSock, set);
        }     
    // }
}

int main(int argc, char const *argv[])
{
	int servSock;
    fd_set socks;
    fd_set readsocks;
    int maxsock;
    int reuseaddr = 1; /* True */

    servSock = CreateTCPServerSocket(PORT);

    /* Set up the fd_set */
    FD_ZERO(&socks);
    FD_SET(servSock, &socks);
    // fcntl(servSock, F_SETFD, O_NONBLOCK); // non-blocking
    maxsock = servSock;

    while (1) {
        unsigned int soc;
        readsocks = socks;
        // printf("run select()\n");
        if (select(maxsock + 1, &readsocks, NULL, NULL, NULL) == -1) {
            error("select");
            return 1;
        }
        for (soc = 0; soc <= maxsock; soc++) {
            if (FD_ISSET(soc, &readsocks)) {
                // printf("socket %d was ready\n", soc);
                if (soc == servSock) {
                    /* New connection */
                    int newsock;
                    struct sockaddr_in their_addr;
                    // size_t size = sizeof(struct sockaddr_in);
                    unsigned int size = sizeof(their_addr);
                    newsock = accept(servSock, (struct sockaddr*)&their_addr, &size);
                    if (newsock == -1) {
                        perror("accept");
                    }
                    else {
                        printf("Got a connection from %s on port %d\n", 
                                inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
                        FD_SET(newsock, &socks);
                        // fcntl(newsock, F_SETFD, O_NONBLOCK); // non-blocking
                        if (newsock > maxsock) {
                            maxsock = newsock;
                        }
                    }
                }
                else {
                    /* Handle read or disconnection */
                    handle(soc, &socks);
                }
            }
        }

    }
	return 0;
}
