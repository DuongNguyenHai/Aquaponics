// lib/seed-branch.cc
// Nguyen Hai Duong
// Date : Nov 2016

#include "seed-branch.h"

namespace TREE {

#define LOGFILE "seed-branch.log" // the file is used for write log
static int DEBUG_LEVEL = 2; // DEBUG_DATABASE_LV was defined in seed-config.cc

static std::string header[] = {"device","data"};
static std::string device[] = {"tempt","DO","amoniac","nitrit","nitrat","hardness"};

Twig::Twig() {}
Twig::~Twig() {} // must has this function for the case multi class use base class
Leaf::~Leaf() {}

Branch::Branch() {}
Branch::~Branch() {} // must has this function for the case multi class use base class

void Branch::Start() {
    Workspace::Start(HandleBranch, BRANCH_PORT);
    SEED_LOG << "Start";
}

inline void CheckCommand(char *str);
// inline void InitialTwig(char *str);

void HandleBranch(int clntSock, fd_set *set) {

    int recvMsgSize;                    /* Size of received message */
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);

    recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
    if (recvMsgSize < 0) 
        SEED_ERROR << "Error reading from socket";
    else if (recvMsgSize>0){
        SEED_VLOG << ". Branch["<< clntSock-3<<"]: sending"; // abstract 3 for convenience
        // CheckCommand(buffer);

        JsonIsValid(buffer);
        int index = JsonGetHeader(buffer, header, sizeof(header)/sizeof(header[0]));
        if(index>=0) {
            SEED_LOG << "The command is : " << header[index].c_str();
            if(index==0) {
                InitialTwig(leaf, buffer);
            }
        }
        else
            SEED_LOG << "Not found command";


        bzero(buffer,strlen(buffer));
    }
    else {
        SEED_LOG << ". Branch["<< clntSock-3<<"]: disconnected";
        close(clntSock);
        FD_CLR(clntSock, set);
    }     
}

// inline void CheckCommand(char *str) {
//     JsonIsValid(str);
//     int index = JsonGetHeader(str, header, sizeof(header)/sizeof(header[0]));
//     if(index>=0) {
//         SEED_LOG << "The command is : " << header[index].c_str();
//         if(index==0) {
//             // Branch::leaf.name = "dfsdf";
//             Branch::InitialTwig(str);
//         }
//     }
//     else
//         SEED_LOG << "Not found command";
// }   

void InitialTwig(Leaf lonlyleaf, char *str) {
    Json *root = JsonParse(str);
    Json *device = JsonGetItem(root, header[0].c_str());
    for (int i = 0 ; i < JsonArraySize(device); i++) {
        cJSON * subitem = JsonGetArrayItem(device, i);
        printf("%s\n", JsonItemVal(subitem));
    }
    // printf("leafs name : %s\n", leaf.name);
}


}	// end of namespace TREE