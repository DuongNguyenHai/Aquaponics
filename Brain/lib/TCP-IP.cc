// lib/TCP-IP.cc

#include "TCP-IP.h"

int CreateTCPServerSocket(unsigned short port){
    int servSock;                           /* socket to create */
    struct sockaddr_in serv_addr;           /* Local address */

    // Create socket for incoming connections 
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        sd_error("ERROR opening socket");
      
    // Construct local address structure 
    memset(&serv_addr, 0, sizeof(serv_addr));       /* Zero out structure */
    serv_addr.sin_family = AF_INET;                 /* Internet address family */
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    serv_addr.sin_port = htons(port);               /* Local port */

    // Bind to the local address
    if (bind(servSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        sd_error("ERROR on binding");

    // Mark the socket so it will listen for incoming connections 
    if (listen(servSock, MAXPENDING) < 0)
        sd_error("ERROR on binding");

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
        sd_error("accept() failed");
      
    return clntSock;
}