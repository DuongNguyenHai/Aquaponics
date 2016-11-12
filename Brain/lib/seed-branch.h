// lib/seed-workspace.h
// Nguyen Hai Duong
// Date : Nov 2016

#ifndef SEED_BRANCH_H
#define SEED_BRANCH_H

#include <iostream>
#include <vector>
#include "seed-config.h"
#include "seed-log.h"
#include "seed-json.h"
#include "TCP-IP.h"
#include "seed-workspace.h"

namespace TREE {

class Leaf {
public:
	Leaf();
	~Leaf();
	char *name;
	bool state;
	void Initial();
};

class Twig {

public:
	Twig();
	~Twig();
	// std::vector<Leaf> lonlyLeaf;
	int totalLeaft;
	bool state;
};

class Branch : public Workspace {

public:
	Branch();
	~Branch();
	std::vector<Twig> lonelyTwig;	// initial a twig (it contains all type of sensor)
	Leaf leaf;
	char *mmm;
	void Start();
	void ConnectToDatabase();

	// friend void InitialTwig(Leaf lonlyleaf,char *str);
private:

	friend void HandleBranch(int clntSock, fd_set *set);
	// Database *dt;
	
};

void HandleBranch(int clntSock, fd_set *set);
// void InitialTwig(Leaf lonlyleaf, char *str);

}	// end of namespace TREE


#endif