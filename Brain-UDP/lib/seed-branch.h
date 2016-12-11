// lib/seed-branch.h
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

#define BRANCH_READ 0
#define REQUEST_WRITE 1

#define RQ_SUCCESS 1
#define RQ_FAIL 0
#define RQ_DATA (char *)"data"
#define TIME_OUT_READ_PIPE 10 // min is 5 seconds
#define VOID_TO_INT(ARG) *( (int *)&ARG)
// Leaf is a sensor
class Leaf {

public:
	Leaf();
	~Leaf();
	char *name;	// the name of sensor
	bool state; // state of sensor
};

// Twig are groups of sensor which are the same type. ex: group of temperature ...  
class Twig {

public:
	Twig();
	~Twig();
	std::vector<Leaf> leaves;
	char *name; // the name of twig
	bool state;	// state of twig.
};

// Branch as a master in real aquaponic system.
class Branch : public Workspace {

public:
	Branch();
	~Branch();
	void Start();		// start a branch. Have not used it.
	static bool IsConnect(){ return flag_twigs; };
private:
	static bool SetTwigs(int clntSock, char *str, const char *header);	// Initial twigs
	static bool CheckUpdateTwig(char *str);	// Update twigs. Actually. I update twig by clearing Twigs and SetTwigs(). This function was written for future when need to deal with each updating of sensor
	static bool SaveDataToTwig(char *str); // save data to twig (collection)
	static bool HandleCommand(int clntSock, char *str); // Handle command corresponds to "set/update/data" commands. 
										  // "set" means: set new groups of sensor (twig). "update" : update sensor from twigs
										  // "data" : save data from all sensor (leaf).
	static void HandleBranch(int clntSock, fd_set *set); // Get data from master through TCP. 
	static void CreateAConnectionToMaster(int clntSock); // Get data from master through UDP
	static void PrintTwigs();	// Print twigs
	static void *RequestDataFromMaster(void *var);
	static void RequestDataFromMasterUDP(int clntSock, struct sockaddr_in serv_addr);
	static bool CheckRequestIsSuccess(int clntSock, struct sockaddr_in serv_addr);
	static Database dt;
	static unsigned int cycle;
	static bool flag_twigs;
};

}	// end of namespace TREE


#endif