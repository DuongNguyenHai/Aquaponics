// lib/seed-Workspace.h
// Nguyen Hai Duong
// Date : Nov 2016

#ifndef SEED_WORKSPACE_H
#define SEED_WORKSPACE_H

#include <iostream>
#include "TCP-IP.h"
#include "seed-log.h"

namespace TREE {

class Workspace {

public:
	Workspace(void (*Handle)(int, fd_set*), int portNumber);
	~Workspace();
private:
	int port;
	unsigned int childProcCount = 0;
	pid_t processID;
	void CreatNewSpace(void (*Handle)(int, fd_set*));
	void clearFork(pid_t processID, unsigned int childProcCount);
};

}	// end of namespace TREE


#endif