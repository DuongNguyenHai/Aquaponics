// lib/seed-console.h
// Nguyen Hai Duong
// Date : Nov 2016

#ifndef SEED_CONSOLE_H
#define SEED_CONSOLE_H

#include <iostream>
#include "seed-workspace.h"
#include "seed-config.h"
#include "seed-log.h"

namespace TREE {

class Console : public Workspace {
public:
	Console() : Workspace(HandleConsole, CONSOLE_PORT) {};
	~Console();

private:
	static void HandleConsole(int clntSock, fd_set *set);;

};

}	// end of namespace TREE


#endif