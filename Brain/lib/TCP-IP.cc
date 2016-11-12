// Nguyen Hai Duong
// Date : july 2016
// lib/TCP-IP.cc

#include "TCP-IP.h"

namespace TREE { 

#define LOGFILE "seed-TCP.log" // the file is used for write log
static int DEBUG_LEVEL = 2; // DEBUG_DATABASE_LV was defined in seed-config.cc

int CreateTCPServerSocket(unsigned short port){
    int servSock;                           /* socket to create */
    struct sockaddr_in serv_addr;           /* Local address */

    // Create socket for incoming connections 
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        SEED_ERROR << "ERROR opening socket";
      
    // Construct local address structure 
    memset(&serv_addr, 0, sizeof(serv_addr));       /* Zero out structure */
    serv_addr.sin_family = AF_INET;                 /* Internet address family */
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    serv_addr.sin_port = htons(port);               /* Local port */

    int enable = 1;
    if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        SEED_ERROR <<"setsockopt(SO_REUSEADDR) failed";

    // Bind to the local address
    if (bind(servSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        SEED_ERROR << "ERROR on binding";

    // Mark the socket so it will listen for incoming connections 
    if (listen(servSock, MAXPENDING) < 0)
        SEED_ERROR << "ERROR on listening";

    return servSock;
}

int AcceptTCPConnection(int servSock){
    int clntSock;                       /* Socket descriptor for client */
    struct sockaddr_in cli_addr;        /* Client address */
    unsigned int clntLen;               /* Length of client address data structure */

    // Set the size of the in-out parameter 
    clntLen = sizeof(cli_addr);
    
    // Wait for a client to connect 
    if ((clntSock = accept(servSock, (struct sockaddr *) &cli_addr, &clntLen)) < 0)
        SEED_ERROR << "accept() failed";
    
    SEED_VLOG   <<"Got a connection from " << inet_ntoa(cli_addr.sin_addr)
                <<" on port " 
                << htons(cli_addr.sin_port);

    return clntSock;
}

}   // end of namespace TREE
