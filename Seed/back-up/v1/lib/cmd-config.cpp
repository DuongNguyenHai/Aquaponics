#include "cmd-config.h"

#ifdef BUFFSIZE
#undef BUFFSIZE
#define BUFFSIZE 1024
#elif BUFFSIZE
#define BUFFSIZE 1024
#endif

unsigned int childProcCount = 0;
pid_t processID;

void CreatCmdConfig(){
    if ( (processID = fork()) < 0) sd_error("fork() failed");
    else if (processID == 0){
        CreatSocketConfig();
        printf("Process terminates\n");
        exit(0);
    }
    childProcCount++;
    clearFork(processID,childProcCount);
}

void CreatSocketConfig(){
	int servSock;       // fd for socket
    fd_set socks;       // save vector of file-desciptor, it includes original socket
    fd_set readsocks;   // save vector of file-desciptor
    int maxsock;		// the maximum of socket. it will be auto increase if there is a new connection


    servSock = CreateTCPServerSocket(CMDPORT);	// creat a new master socket

    /* Set up the fd_set */
    FD_ZERO(&socks);
    FD_SET(servSock, &socks);
    maxsock = servSock;

    while (1) {
        unsigned int soc;
        readsocks = socks;
        // printf("run select()\n");
        if (select(maxsock + 1, &readsocks, NULL, NULL, NULL) == -1) {
            sd_error("select");
        }
        for (soc = 0; soc <= maxsock; soc++) {
            if (FD_ISSET(soc, &readsocks)) {
                if (soc == servSock) {
                    /* New connection */
                    int newsock;
                    struct sockaddr_in their_addr;
                    unsigned int size = sizeof(their_addr);
                    newsock = accept(servSock, (struct sockaddr*)&their_addr, &size);
                    if (newsock == -1) {
                        sd_error("accept");
                    }
                    else {
                        printf("Got a connection from %s on port %d\n", 
                                inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
                        FD_SET(newsock, &socks);
                        if (newsock > maxsock) {
                            maxsock = newsock;
                        }
                    }
                }
                else {
                    /* Handle read or disconnection */
                    HandleClient(soc, &socks);
                }
            }
        }
    }
}

void HandleClient(int clntSock, fd_set *set){
    int recvMsgSize;                    /* Size of received message */
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);
    
    recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
    if (recvMsgSize < 0) 
        sd_error("ERROR reading from socket");
    else if(recvMsgSize>0){
        printf(". Client[%d]: %s\n",clntSock,buffer);
        bzero(buffer,strlen(buffer));
    }
    else{
        printf("- Client[%d]: disconnected !\n",clntSock);
        close(clntSock);
        FD_CLR(clntSock, set);
    }     
}

bool cmd::hello(){
    printf("hello world !\n");
    return 0;
}