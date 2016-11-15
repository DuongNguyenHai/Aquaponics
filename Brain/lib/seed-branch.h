// lib/seed-workspace.h
// Nguyen Hai Duong
// Date : Nov 2016

#ifndef SEED_BRANCH_H
#define SEED_BRANCH_H

#include <iostream>
#include <vector>
#include "seed-config.h"
#include "seed-parse-config.h"
#include "seed-log.h"
#include "seed-json.h"
#include "seed-database.h"
#include "seed-workspace.h"

namespace TREE {

class Leaf {

public:
	Leaf();
	~Leaf();
	char *name;	// the name of sensor
	bool state;
};

class Twig {

public:
	Twig();
	~Twig();
	std::vector<Leaf> leaves;
	// std::string name;
	char *name; // the name of sensor
	bool state;
};

class Branch : public Workspace, Database {

public:
	Branch();
	~Branch();
	
	void Start();		// start a branch

	void ConnectToDatabase();
	friend bool SetTwigs(char *str, const char *header);	// Initial twigs
	static bool CheckUpdateTwig(char *str);	// Update twigs
	bool SaveDataToTwig(char *str); // save data to twig (collection)
	friend void CheckCommand(char *str);
private:
	friend void HandleBranch(int clntSock, fd_set *set);
	
	// Database dt;
};

bool SetTwigs(char *str, const char *header);
void HandleBranch(int clntSock, fd_set *set);
// bool SaveDataToTwig(char *str);
void CheckCommand(char *str);
}	// end of namespace TREE


#endif