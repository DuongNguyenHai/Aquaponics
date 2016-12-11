// lib/seed-workspace.cc
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

// Define a file which is used for write log

// #ifdef LOGFILE
// #undef LOGFILE
// #define LOGFILE "../log/seed-workspace.log"
// #else
// #define LOGFILE "../log/seed-workspace.log"
// #endif

unsigned int Workspace::childProcCount = 0;
// int Workspace::indexOfPine = 0;
int Workspace::fd[2*MAX_PIPES] = {0};

static std::vector<struct threadAndClient> threadAndClnt;

Workspace::Workspace() {}

Workspace::~Workspace() {
    clearFork();
}

void Workspace::CreateANewOnlineSpace(void (*Handle)(int, fd_set*), int port, void *(*func)(void *)) {
    if ( (processID = fork()) == -1 ) 
        SEED_ERROR << "Cant create a new Workspace cause fork() failed";
    else if (processID == 0) {
        childProcCount++;
        PutOnline(Handle, port, func);
    }
}

void Workspace::CreateANewOnlineSpace(void (*Handle)(int), const char *servIP, int port, void (*func)(int, struct sockaddr_in)) {
    if ( (processID = fork()) == -1 ) 
        SEED_ERROR << "Cant create a new Workspace cause fork() failed";
    else if (processID == 0) {
        childProcCount++;
        PutOnlineUDP(Handle, servIP, port, func);
    }
}


void Workspace::CreateANewSpace(void (*func)()) {
    if ( (processID = fork()) == -1 ) 
        SEED_ERROR << "Cant create a new Workspace cause fork() failed";
    else if (processID == 0) {
        childProcCount++;
        func();
    }
}


void Workspace::CreateANewWork(void *(*func)(void *), void *var) {

    threadAndClnt.resize(threadAndClnt.size()+1);   // increase a threadAndClnt size
    threadAndClnt.back().sock = VOID_TO_INT(var);   // save newsock to threadAndClnt

    if (pthread_create(&threadAndClnt.back().threadID, NULL, func, var))
        SEED_WARNING << "pthread_create() failed";

    // PrintVectorThreadAndClient();
}

// NOT use this function.
int Workspace::CreateAPairOfPine(int num) {
    num--; // convert to real index of fd[]
    if( fd[num] !=0 || fd[num+1] !=0 ) {
        SEED_WARNING << "The pair of descriptor from [" << num << "] was created";
        return RET_FAILURE;
    }
    if (pipe(fd+(num*2)) < 0) {
        SEED_WARNING << "Failed to allocate pipes";
        return RET_FAILURE;
    }
    // SEED_LOG << "fd["<<num<<"]: " << fd[num] << ", fd["<<num+1<<"]: " << fd[num+1];
    return RET_SUCCESS; 
}

void Workspace::PutOnline(void (*Handle)(int, fd_set*), int port, void *(*func)(void *)) {

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
        if (select(maxsock + 1, &readsocks, NULL, NULL, NULL) == -1) {
            SEED_ERROR << "Error on select";
        }
        for (soc = 0; soc <= maxsock; soc++) {
            if (FD_ISSET(soc, &readsocks)) {
                if (soc == servSock) {
                    /* New connection */
                    int newsock = AcceptTCPConnection(servSock);

                    // show address of master which connected 
                    struct sockaddr_in cli_addr;
                    unsigned int clntLen = sizeof(cli_addr);
                    getpeername(newsock, (struct sockaddr *) &cli_addr, &clntLen);
                    SEED_LOG << "Branch["<<inet_ntoa(cli_addr.sin_addr)<<"] has created";

                    if(func!=NULL) {
                        Workspace::CreateANewWork(func, (void*)(intptr_t)newsock);
                    }

                    if (newsock == -1) {
                        SEED_ERROR << "Error on accept a new connection";
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

void Workspace::PutOnlineUDP(void (*Handle)(int), const char *servIP, int port, void (*func)(int, struct sockaddr_in)) {
    struct sockaddr_in serv_addr;
    int clntSock = CreateUDPClientSocket(serv_addr, servIP, port);
    // Its not good when calling "request function" before "get data function". Cause in rarely situation, "request function" implement request befor recvfrom() was run in "get data funcion"
    // func() its a request function to master

    if ( (processID = fork()) == -1 ) 
        SEED_ERROR << "Cant create a new Workspace cause fork() failed";
    else if (processID == 0) { // child
        childProcCount++;
        func(clntSock, serv_addr);
    }
    else { // parent
        Handle(clntSock); // its a get data function from master
    }
    
}

void Workspace::clearFork() {
    while (childProcCount){
        processID = waitpid((pid_t)-1, NULL, WNOHANG); /* Nonblocking wait */
        if (processID < 0) /* waitpid() sd_error? */
            SEED_ERROR << "waitpid() failed";
        else if (processID == 0) {  /* No zombie to wait on */
            break;
        }
        else {
            childProcCount--; /* Cleaned up after a child */
        }
    }
}

void Workspace::ClearThread(int sock) {

    for (unsigned int i = 0; i < threadAndClnt.size(); ++i) {
        if (sock==threadAndClnt[i].sock)
        {
            pthread_cancel(threadAndClnt[i].threadID);
            for (unsigned int j = i; j < threadAndClnt.size()-1; ++j)
            {
                threadAndClnt[j].sock = threadAndClnt[j+1].sock;
                threadAndClnt[j].threadID = threadAndClnt[j+1].threadID;
            }
        }
    }
    threadAndClnt.pop_back();
    // PrintVectorThreadAndClient();
}

void Workspace::PrintVectorThreadAndClient() {
    SEED_LOG << "threadAndClient size: " << threadAndClnt.size();
    for (unsigned int i = 0; i < threadAndClnt.size(); ++i) {
        SEED_LOG << "threadID: " << threadAndClnt[i].threadID << ", sock: " << threadAndClnt[i].sock;
    }
}

}	// end of namespace TREE