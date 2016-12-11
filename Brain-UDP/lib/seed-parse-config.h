// Nguyen Hai Duong
// Nov 2016

#ifndef SEED_PARSE_CONFIG_H
#define SEED_PARSE_CONFIG_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <inttypes.h>	// int32_t, uint32_t
#include <unistd.h>
#include <algorithm>	// remove
#include <sys/param.h>	/* For PATH_MAX (the maximum length of a path). */
#include <arpa/inet.h>  // for sockaddr_in and inet_aton()
#include "seed-log.h" // Print information and write to a log-file
#include "seed-config.h" // configuration for system
#include "seed-json.h"

namespace TREE {

#define NUM_ROW(ARG) (sizeof(ARG)/sizeof(ARG[0][0])/2)

char *call_realpath (const char *argv0);

class ParseFile {

public:
	ParseFile(const char *file);
	~ParseFile();
	bool ParseArgs(std::string args[][2], int maxSize);
	void PrintUsage();
	void PrintConfig();

private:
	int index;
	std::ifstream pfile;
	std::string fileName;
	std::string str;
};

class ParseOptions : public ParseFile {

public:
	ParseOptions(const char *file);
	~ParseOptions();
	void GetOptions();
	void PrintUsage();
};

}	// end of namespace TREE


#endif