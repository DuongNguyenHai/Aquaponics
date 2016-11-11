// lib/seed-workspace.h
// Nguyen Hai Duong
// Date : Nov 2016

#ifndef SEED_BRANCH_H
#define SEED_BRANCH_H

#include <iostream>
#include "TCP-IP.h"
#include "seed-workspace.h"
#include "seed-config.h"
#include "seed-log.h"

namespace TREE {

class Branch : public Workspace {
public:
	Branch() : Workspace(HandleBranch, BRANCH_PORT) {}
	~Branch();

private:
	static void HandleBranch(int clntSock, fd_set *set);;

};

}	// end of namespace TREE


#endif