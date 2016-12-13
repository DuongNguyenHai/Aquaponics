#include <stdio.h>
#include <iostream>
#include "seed.h"
#include <unistd.h>

using namespace TREE;

#define LOG_FILE "config/root.conf"
static std::string GetRootDir(const char *argv0);

int main(int argc, char const *argv[]) {

    
    std::string rootDir = GetRootDir(argv[0]);
    std::string configFile (rootDir);
    configFile.append(LOG_FILE);
    
    ParseOptions po(configFile.c_str());
    SEED_LOG << "___________START AQUAOS ROOT___________";
    SEED_LOG << "Master address: " << MASTER_IP << ", port: " << BRANCH_PORT;
    Branch SongHe; // Initial a new branch
    
    // Config for branch
    SongHe.Start(); // start the branch

    // Must include pause() or wait(NULL) to terminate all child process (fork()) when main process was shutdowned
    
    // wait for child termination
    wait(NULL);

    pause();	// suspend the calling thread until delivery of a signal whose action is either to execute a signal-catching function or to terminate the process
    // pause() is not safe in some case. Think about sigsuspend()

	return 0;
}

static std::string GetRootDir(const char *argv0) {
    std::string mainPath = call_realpath(argv0);
    std::size_t found = mainPath.find_last_of("/");
    std::string rootDir = mainPath.substr(0,found);
    found = rootDir.find_last_of("/");
    return rootDir.substr(0,found+1);
}