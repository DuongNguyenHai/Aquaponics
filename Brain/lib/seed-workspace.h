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
	void Start(void (*Handle)(int, fd_set*), int portNumber);
private:
	int port;
	unsigned int childProcCount = 0;
	pid_t processID;
	
	void CreatNewSpace(void (*Handle)(int, fd_set*));
	void clearFork(pid_t processID, unsigned int childProcCount);
};

}	// end of namespace TREE


#endif