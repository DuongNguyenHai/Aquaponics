// lib/seed-Workspace.h
// Nguyen Hai Duong
// Date : Nov 2016

#ifndef SEED_WORKSPACE_H
#define SEED_WORKSPACE_H

#include <iostream>
#include "seed-json.h" // Print information and write to a log-file
#include "TCP-IP.h"
#include "seed-log.h"
#include "seed-database.h"

namespace TREE {

class Workspace {

public:
	Workspace();
	~Workspace();
	void CreateANewOnlineSpace(void (*Handle)(int, fd_set*), int port);
	void CreateANewSpace(void (*func)());
private:
	int servSock; // fd for socket
	static unsigned int childProcCount;
	pid_t processID;
	
	void PutOnline(void (*Handle)(int, fd_set*), int port);
	void clearFork();
};

}	// end of namespace TREE


#endif