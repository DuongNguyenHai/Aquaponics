#ifndef TCP_IP_H
#define TCP_IP_H

#include <stdio.h>		// for printf() and fprintf()
#include <sys/socket.h> // for socket(), bind(), and connect()
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>		// for memset()
#include <unistd.h>		// for sleep(), close()
#include <sys/types.h>	// for Socket data types
#include <netinet/in.h> // for IP Socket data types
#include <pthread.h>	// for multithread function
#include <sys/wait.h>	// for waitpid()

#include "seed-log.h"

#define MAXPENDING 5
#define BUFFSIZE 256
#define PORT 8888
// handle client
void *HandleThreadClient(void *socket_desc);

// creat tco server socket
int CreateTCPServerSocket(unsigned short port);

// accept tcp connection
// Return server socket
int AcceptTCPConnection(int servSock);

// TCP client handling
void HandleForkClient(int clntSocket);

// Clean up all zombies form fork 
void clearFork(pid_t processID, unsigned int childProcCount);

#endif