#ifndef CMD_CONFIG_H
#define CMD_CONFIG_H

#include "TCP-IP.h"
#include "seed-log.h"

#define CMDPORT 8888

// The function is used to creat a new fork process to deal with config
void CreatCmdConfig();

void CreatSocketConfig();

void HandleClient(int clntSock, fd_set *set);

extern unsigned int childProcCount;	// it was defined in cmdConfig.cpp
extern pid_t processID;				// it was defined in cmdConfig.cpp
class cmd
{
public:
	bool hello();
	
};

#endif