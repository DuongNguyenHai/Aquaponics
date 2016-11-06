// Seed-TCP/server-fork.cc

// Nguyen Hai Duong
// Date : 11 jun 2016

// compile : g++ server-fork.cc ../lib/TCP-IP.cc ../lib/seed-log.cc -o server-fork

#include "../lib/TCP-IP.h"


inline void HandleForkClient(int clntSock);
inline void clearFork(pid_t processID, unsigned int childProcCount);

int main(int argc, char const *argv[]){
    
	int servSock, clntSock;
    struct sockaddr_in cli_addr;    // struct contain clients inform
    unsigned int clntLen;           // Length of client address data structure
    pid_t processID;
    unsigned int childProcCount = 0;

    clntLen = sizeof(cli_addr);

    servSock = CreateTCPServerSocket(PORT);

    while(1){

        clntSock = AcceptTCPConnection(servSock);
        getpeername(clntSock, (struct sockaddr *) &cli_addr, &clntLen);
        /* Fork child process and report any errors */
        if ( (processID = fork()) < 0)
            perror("fork() failed");
        else if (processID == 0) /* This is the child process */
        {
            close(servSock);
            /* Child closes listening socket */
            printf("\n+ New client[%d][Addr:%s][Port:%d]\n\n", 
            clntSock, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

            HandleForkClient(clntSock);
            exit(0);
            /* Child process terminates */
        }/* Child closes listening socket*/
        
        // printf("processID : %d\n", (int) processID);
        close(clntSock);
        /* Parent closes child socket descriptor */
        childProcCount++;
        /* Increment number of outstanding child processes */
        while (childProcCount) /* Clean up all zombies */
        {
            processID = waitpid((pid_t) -1, NULL, WNOHANG); /* Nonblocking wait */
            if (processID < 0) /* waitpid() error? */
                perror("waitpid() failed");
            else if (processID == 0) /* No zombie to wait on */
                {
                    break;
                }
                else{
                    childProcCount--; /* Cleaned up after a child */
                }
        }
    }

	return 0;
}

inline void HandleForkClient(int clntSock){
    int recvMsgSize;                    /* Size of received message */
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);
    
    while(1){
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

inline void clearFork(pid_t processID, unsigned int childProcCount){
    while (childProcCount){
        processID = waitpid((pid_t) -1, NULL, WNOHANG); /* Nonblocking wait */
        if (processID < 0) /* waitpid() sd_error? */
            sd_error("waitpid() failed");
        else if (processID == 0) {  /* No zombie to wait on */
            break;
        }
        else{
            childProcCount--; /* Cleaned up after a child */
        }
    }
}