// lib/seed-Workspace.cc
// Nguyen Hai Duong
// Date : Nov 2016

#include "seed-workspace.h"

namespace TREE {

#ifdef BUFFSIZE
#undef BUFFSIZE
#define BUFFSIZE 1024
#elif BUFFSIZE
#define BUFFSIZE 1024
#endif

#define LOGFILE "seed-workspace.log" // the file is used for write log
static int DEBUG_LEVEL = 2; // DEBUG_DATABASE_LV was defined in seed-config.cc

Workspace::Workspace() {}
Workspace::~Workspace() {
	clearFork(processID, childProcCount);
}

void Workspace::Start(void (*Handle)(int, fd_set*), int portNumber) {

    port = portNumber;
    if ( (processID = fork()) < 0) 
        SEED_ERROR << "fork() failed";
    else if (processID == 0) {
        childProcCount++;
        CreatNewSpace(Handle);
        exit(0);
    }

}

void Workspace::CreatNewSpace(void (*Handle)(int, fd_set*)) {
    int servSock;       // fd for socket
    fd_set socks;       // save vector of file-desciptor, it includes original socket
    fd_set readsocks;   // save vector of file-desciptor
    int maxsock;        // the maximum of socket. it will be auto increase if there is a new connection


    servSock = CreateTCPServerSocket(port);  // creat a new master socket

    /* Set up the fd_set */
    FD_ZERO(&socks);
    FD_SET(servSock, &socks);
    maxsock = servSock;

    while (1) {
        int soc;
        readsocks = socks;
        // printf("run select()\n");
        if (select(maxsock + 1, &readsocks, NULL, NULL, NULL) == -1) {
            SEED_ERROR << "Error on select";
        }
        for (soc = 0; soc <= maxsock; soc++) {
            if (FD_ISSET(soc, &readsocks)) {
                if (soc == servSock) {
                    /* New connection */
                    int newsock = AcceptTCPConnection(servSock);
                    if (newsock == -1) {
                        SEED_ERROR << "Error on accept";
                    }
                    else {
                        
                        FD_SET(newsock, &socks);
                        if (newsock > maxsock) {
                            maxsock = newsock;
                        }
                    }
                }
                else {
                    /* Handle read or disconnection */
                    Handle(soc, &socks);
                }
            }
        }
    }
}

void Workspace::clearFork(pid_t processID, unsigned int childProcCount) {
    while (childProcCount){
        processID = waitpid((pid_t) -1, NULL, WNOHANG); /* Nonblocking wait */
        if (processID < 0) /* waitpid() sd_error? */
            SEED_ERROR << "waitpid() failed";
        else if (processID == 0) {  /* No zombie to wait on */
            break;
        }
        else{
            childProcCount--; /* Cleaned up after a child */
        }
    }
}


}	// end of namespace TREE