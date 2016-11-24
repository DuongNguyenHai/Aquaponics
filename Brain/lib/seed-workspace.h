// lib/seed-workspace.h
// Nguyen Hai Duong
// Date : Nov 2016

#ifndef SEED_WORKSPACE_H
#define SEED_WORKSPACE_H

#include <iostream>
#include <vector>
#include <pthread.h>	// for multi thread
#include "seed-json.h" // Print information and write to a log-file
#include "TCP-IP.h"
#include "seed-log.h"
#include "seed-database.h"

namespace TREE {

// #define NUM_PIPES 2	// the number of pine we made
#define VOID_TO_INT(ARG) *( (int *)&ARG)

struct threadAndClient{
	pthread_t threadID;
	int sock;
};


class Workspace {

public:
	Workspace();
	~Workspace();
	void CreateANewOnlineSpace(void (*Handle)(int, fd_set*), int port, void *(*func)(void *)=NULL);
	void CreateANewSpace(void (*func)());	// create a new process
	void CreateANewWork(void *(*func)(void *), void *var);	// create a new thread
	static void PrintVectorThreadAndClient();
	// Make a communication between processes. But it's no longer used.
	// int CreateAPairOfPine(int index);	// create a pair of descriptor for pipe.
	// static int fd[2*NUM_PIPES];
	// static int indexOfPine;
	static void ClearThread(int sock);

private:
	int servSock; // fd for socket
	pid_t processID;
	// static pthread_t threadID;
	static unsigned int childProcCount;
	void PutOnline(void (*Handle)(int, fd_set*), int port, void *(*func)(void *)=NULL);
	void clearFork();
};

}	// end of namespace TREE


#endif