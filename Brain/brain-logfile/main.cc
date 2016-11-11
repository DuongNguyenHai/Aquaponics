#include "seed-log.h" // Print information and write to a log-file
using namespace TREE;

// There are 3 level of DEBUG_LEVEL from 1->3
// Higher level will debug more information
// Set  DEBUG_LEVEL 0 means : show WARNING, ERROR but no LOG.
// 		DEBUG_LEVEL 1 means : show WARNING, ERROR and LOG.
// 		DEBUG_LEVEL 2 means : show all.

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 2
#else
#undef DEBUG_LEVEL
#define DEBUG_LEVEL 2
#endif

#define LOGFILE "main.log"

void foo() {
	SEED_LOG << "What the hell is going on !";
}

int main(int argc, char const *argv[])
{
	
	foo();
	SEED_WARNING << "U should notice on ... !!";
	SEED_ERROR << "There're something wrong !!!";
	SEED_VLOG << "Show more information.";
	return 0;
}