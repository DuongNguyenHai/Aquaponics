#include <stdio.h>
#include <iostream>
#include "seed-config.h"
#include "seed-log.h"
#include "seed-parse-config.h"
#include "seed-branch.h"
#include <unistd.h>

using namespace TREE;

int main(int argc, char const *argv[]) {


    ParseOptions po("../config/sys-config.cfg");
    
    Branch SongHe; // Initial a new branch
    // Config for branch
    SongHe.InitalRequest(INTERVAL_REQUEST); // func : InitalRequest(time) , unit : second
    SongHe.Start(); // start the branch


    // Must include pause() or wait(NULL) to terminate all child process (fork()) when main process was shutdowned
    
    // wait for child termination
    // wait(NULL);

    pause();	// suspend the calling thread until delivery of a signal whose action is either to execute a signal-catching function or to terminate the process
    // pause() is not safe in some case. Think about sigsuspend()

	return 0;
}
