// Nguyen Hai Duong
// Nov 2016

#ifndef SEED_CONFIG_H
#define SEED_CONFIG_H

#include <iostream>

namespace TREE {

extern bool PRINT_MONITOR;  // print infor on terminal
extern bool PRINT_FILE;     // print infor to file (log file)

// There are 3 level of debug level  from 1->3
// Higher level will debug more information
// Set  DEBUG_..._LV 0 means : show WARNING, ERROR but no LOG.
// 		DEBUG_..._LV 1 means : show WARNING, ERROR and LOG.
// 		DEBUG_..._LV 2 means : show WARNING, ERROR and LOG and VERBOSE.
// ex : int DEBUG_DATABASE_LV = 1; // note : define in seed-config.cc
extern int DEBUG_DATABASE_LV;

// Type of return. All function of system will return one of three types.
#define RET_MISS -1
#define RET_SUCCESS 0
#define RET_FAILURE 1

// Define args to config : 
// "0" : string, "1" : bool, "2" : int, "3" : float, "4" : double ...
// 
extern std::string DEFINE_ARGS[3][2];

#define Tp_STR	"0"
#define Tp_BOOL "1"
#define Tp_INT	"2"
#define Tp_FLOAT "3"
#define Tp_DOUBLE "4"

extern const char *usage;

}	// end of namespace TREE


#endif