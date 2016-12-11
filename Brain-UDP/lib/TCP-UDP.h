// Nguyen Hai Duong
// Date : july 2016
// lib/TCP-UDP.h

#ifndef SEED_TCP_IP_H
#define SEED_TCP_IP_H

#include <sys/socket.h> // for socket(), bind(), and connect()
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>		// for memset()
#include <unistd.h>		// for sleep(), close()
#include <sys/types.h>	// for Socket data types
#include <netinet/in.h> // for IP Socket data types
#include <pthread.h>	// for multithread function
#include <sys/wait.h>	// for waitpid()
#include <sys/time.h>   // for struct timeval()
#include <fcntl.h>      // for fcntl()

// for log and error
#include "seed-log.h"

namespace TREE {

#define MAXPENDING 5
#define BUFFSIZE 256

// Create tcp server socket
int CreateTCPServerSocket(unsigned short port);

// accept tcp connection
// Return server socket
int AcceptTCPConnection(int servSock);

// Initial UDP client socket
int CreateUDPClientSocket(struct sockaddr_in &serv_addr, const char *servIP, unsigned short port);

// Initial UDP server socket
int CreateUDPServerSocket(unsigned short port);


}	// end of name space TREE

#endif