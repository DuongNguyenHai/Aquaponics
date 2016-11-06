// lib/seed-console.cc
// Nguyen Hai Duong
// Date : Nov 2016

#include "seed-console.h"

namespace TREE {

#define LOGFILE "seed-console.log" // the file is used for write log
static int DEBUG_LEVEL = 2; // DEBUG_DATABASE_LV was defined in seed-config.cc

Console::~Console() {} // must has this function for the case multi class use base class

void Console::HandleConsole(int clntSock, fd_set *set) {

    int recvMsgSize;                    /* Size of received message */
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);
    
    recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
    if (recvMsgSize < 0) 
        SEED_ERROR << "Error reading from socket";
    else if(recvMsgSize>0){
        SEED_LOG << ". Console["<< clntSock-3<<"]: " << buffer; // abstract 3 for convenience
        bzero(buffer,strlen(buffer));
    }
    else{
        SEED_LOG << ". Console["<< clntSock-3<<"]: disconnected";
        close(clntSock);
        FD_CLR(clntSock, set);
    }     
}

}   // end of namespace TREE