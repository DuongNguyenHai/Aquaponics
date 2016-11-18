// Seed-TCP/server-thread.cc

// Nguyen Hai Duong
// Date : 11 jun 2016

// compile : g++ -o server-thread server-thread.cc ../lib/TCP-IP.cc ../lib/seed-log.cc -lpthread


#include "../lib/TCP-IP.h"

// Socket descriptor for client
struct ThreadArgs{
    int clntSock;
};

inline void *HandleThreadClient(void *threadArgs);

int main(int argc, char *argv[]){

    int servSock, clntSock;
    struct sockaddr_in cli_addr;
    pthread_t threadID;              /* Thread ID from pthread_create() */
    struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
    unsigned int clntLen;            /* Length of client address data structure */

    clntLen = sizeof(cli_addr);

    servSock = CreateTCPServerSocket(PORT);

    while(1) {

        clntSock = AcceptTCPConnection(servSock);

        getpeername(clntSock, (struct sockaddr *) &cli_addr, &clntLen);

        // Create separate memory for client argument

        if ((threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs))) == NULL)
            perror("malloc() failed");

        threadArgs -> clntSock = clntSock;

        if (pthread_create(&threadID, NULL, HandleThreadClient, (void *) threadArgs) != 0)
            perror("pthread_create() failed");

        printf("\n+ New client[%d][Addr:%s][Port:%d]\n\n", 
            clntSock, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    }

    close(servSock);
    return 0; 
}

inline void *HandleThreadClient(void *threadArgs) {
    
    int clntSock;
    int recvMsgSize;
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);

    clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
    
    while(1) {
        recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
        if (recvMsgSize < 0) 
            sd_error("ERROR reading from socket");
        else if(recvMsgSize>0){
            printf(". Client[%d]: %s\n",clntSock,buffer);
            bzero(buffer,strlen(buffer));
        }
        else{
            printf("- Client[%d]: disconnected !\n",clntSock);
            break;
        }     
    }
    close(clntSock);
}
