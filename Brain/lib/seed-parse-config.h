// Nguyen Hai Duong
// Nov 2016

#ifndef SEED_PARSE_CONFIG_H
#define SEED_PARSE_CONFIG_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <inttypes.h>	// int32_t, uint32_t
#include <algorithm>	// remove

#include "seed-log.h" // Print information and write to a log-file
#include "seed-config.h" // configuration for system

namespace TREE {

// config-file: <option> = <content>
// ex in config file:
// PRINT_MONITOR=true
// DEBUG_LEVEL=2
// # comment line

#define NUM_ROW(ARG) (sizeof(ARG)/sizeof(ARG[0][0])/2)

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