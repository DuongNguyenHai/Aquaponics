#include <stdio.h>
#include <iostream>
#include "seed-config.h"
#include "seed-log.h"
#include "seed-parse-config.h"
#include "seed-branch.h"
#include <unistd.h>

using namespace TREE;

int main(int argc, char const *argv[])
{
	// Set config for system
    ParseOptions po("../config/sys-config.cfg");

    Branch SongHe;
    
    pause();	// suspend the calling thread until delivery of a signal whose action is either to execute a signal-catching function or to terminate the process
    // pause() is not safe in some case. Think about sigsuspend()
    
    // while (!usr_interrupt)
  		// sleep (1);

	return 0;
}