// lib/seed-branch.cc
// Nguyen Hai Duong
// Date : Nov 2016

#include "seed-branch.h"

namespace TREE {

// define the file is used for write log

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE "seed-branch.log"
#else
#define LOGFILE "seed-branch.log"
#endif

#define OK "OK"

static std::string header[] = {"set","update","data"};
static std::string device[] = {"tempt","DO","amoniac","nitrit","nitrat","hardness"};
static std::vector<Twig> twigs;   // initial a twig (it contains all name of sensor)
unsigned int Branch::cycle = 0;
bool Branch::flag_twigs = false;
// Check if str is the same to vec.name
template <typename T>
static bool CheckEqualVector(std::vector<T> &vec, char *str);
static bool SendStringToMaster(int clntSock, char *str);

Leaf::Leaf() {}
Leaf::~Leaf() {
    free(name);
}

Twig::Twig() {}
Twig::~Twig() { 
    leaves.clear();
    free(name);
} // must has this function for the case multi class use base class

Branch::Branch() {}
Branch::~Branch() {}

Database Branch::dt(DATABASE_NAME);

void Branch::Start() {

    if(Branch::cycle>0)  {// check interval time for request
        Workspace::CreateANewOnlineSpace(HandleBranch, BRANCH_PORT, RequestDataFromMaster);
    }
    else {
        SEED_WARNING << "Interval of Request() = "<<Branch::cycle << ". It must be larger > 0";
        Workspace::CreateANewOnlineSpace(HandleBranch, BRANCH_PORT);
    }
}

void Branch::HandleBranch(int clntSock, fd_set *set) {

    int recvMsgSize;                    /* Size of received message */
    char buffer[BUFFSIZE];
    struct sockaddr_in cli_addr;
    unsigned int clntLen = sizeof(cli_addr);
    getpeername(clntSock, (struct sockaddr *) &cli_addr, &clntLen);

    recvMsgSize = recv(clntSock, buffer, BUFFSIZE, 0);
    if (recvMsgSize < 0) 
        SEED_WARNING << "Error reading from socket";
    else if (recvMsgSize>0){
        SEED_VLOG << "Branch["<<inet_ntoa(cli_addr.sin_addr)<<"] is sending a message"; // abstract 3 for convenience
        Branch::HandleCommand(clntSock, buffer);
        bzero(buffer,strlen(buffer));   // need bzero to clear buffer. if dont do that, the next time will print some old stuffs at the end. Still dont know why
    }
    else {
        SEED_LOG << "Branch["<<inet_ntoa(cli_addr.sin_addr)<<"] has disconnected";
        close(clntSock);
        Workspace::ClearThread(clntSock);
        FD_CLR(clntSock, set);
    }

}

bool Branch::HandleCommand(int clntSock, char *str) {

    if(JsonIsValid(str)==RET_FAILURE)
        return RET_FAILURE;

    int index = JsonGetHeader(str, header, sizeof(header)/sizeof(header[0])); // compare with header[] list to check what command is ?
    if(index>=0) {

        SEED_VLOG << "The command of Branch is : " << header[index].c_str();
        switch(index) {
            case 0 : {  // set sensor device (twig) to branch
                // Branch::SetTwigs(clntSock, str, header[0].c_str());
                // SendStringToMaster(clntSock , (char *)OK);   // I have no longer to use it. it informs "OK" to master. it represent that everything was successed
                // Branch::PrintTwigs();
                
            } break;
            case 1 : {  // update sensor device (twig) to branch
                // Branch::CheckUpdateTwig(str);
                // Clear all leaves in twigs and set new leaves
                // Branch::SetTwigs(clntSock, str, header[1].c_str());
                // SendStringToMaster(clntSock , (char *)OK);   // I have no longer to use id
                // Branch::PrintTwigs();
            } break;
            case 2 : { // save data from sensor device (twig)
                Branch::SaveDataToTwig(str);
                // SendStringToMaster(clntSock , (char *)OK);
            } break;
            default: break;
        }
        return RET_SUCCESS;
    }
    else {
        SEED_WARNING << "Not found command of branch";
        return RET_FAILURE;
    }
}   

bool Branch::SetTwigs(int clntSock, char *str, const char *header) {

    twigs.clear();
    Json *root = JsonParse(str);
    Json *jnDevice = JsonGetObjectItem(root, header);  // get item form device
    int sizeJnDevice = JsonArraySize(jnDevice);
    twigs.resize(sizeJnDevice);
    cJSON *item, *subitem;
    // Create twigs
    for (int i = 0 ; i < sizeJnDevice; ++i) {
        item = JsonGetArrayItem(jnDevice, i);
        
        twigs[i].state = true;
        twigs[i].name = strdup(JsonItemName(item));
        twigs[i].leaves.resize(JsonArraySize(item));
        // Create leaves
        for (int j = 0; j < JsonArraySize(item); ++j) {
            subitem = JsonGetArrayItem(item, j);
            twigs[i].leaves[j].name = strdup(JsonItemName(subitem));
            twigs[i].leaves[j].state = JsonItemValBool(subitem);
        }
    }
    
    cJSON_Delete(root);
    flag_twigs = true;
    return RET_SUCCESS;
}

// This function was written for future when really need to deal with each updating of sensor
bool Branch::CheckUpdateTwig(char *str) {

    Json *root = JsonParse(str);
    Json *jnDevice = JsonGetObjectItem(root, header[1].c_str()); // get item form device
    int sizeJnDevice = JsonArraySize(jnDevice);
    Json *item, *subitem;
    
    // Create twigs
    for (int i = 0 ; i < sizeJnDevice; ++i) {
        item = JsonGetArrayItem(jnDevice, i);
        if(CheckEqualVector<Twig>(twigs, JsonItemName(item))) {
            for (int j = 0; j < JsonArraySize(item); ++j) {
                subitem = JsonGetArrayItem(item, j);
                if(CheckEqualVector<Leaf>(twigs[i].leaves, JsonItemName(subitem)));

                else
                    SEED_LOG << "Adding a new sensor \"" << JsonItemName(subitem) << "\" to twig \"" << JsonItemName(item) << "\"";
            }
        }
        else {
            SEED_LOG << "Adding a new twig \"" << JsonItemName(item) << "\" to branch";
        }
    }

    cJSON_Delete(root);
    return RET_SUCCESS;
}

// save data to twig (collection)
bool Branch::SaveDataToTwig(char *str) {

    Json *root = JsonParse(str);
    Json *data = JsonGetObjectItem(root, header[2].c_str()); // get item form data
    Json *leaf = JsonGetArrayItem(data, 0);
    char *out = cJSON_Print(leaf);
    dt.InsertDataWithIdAutoIncrement(JsonItemName(leaf), out);
    cJSON_Delete(root);
    free(out);
    return RET_SUCCESS;
}

void Branch::PrintTwigs() {
    for (unsigned int i = 0; i < twigs.size(); ++i) {
        SEED_LOG << "twig: " << twigs[i].name << ", state: " <<  ((twigs[i].state) ? "alive":"die");
        for (unsigned int j = 0; j < twigs[i].leaves.size(); ++j)
            SEED_LOG << "\t- leaf: " << twigs[i].leaves[j].name << ", state: " <<  ((twigs[i].leaves[j].state) ? "alive":"die");
    }
}

void Branch::InitalRequest(int T) {
    Branch::cycle = T;
}

void *Branch::RequestDataFromMaster(void *var) {

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    int clntSock = VOID_TO_INT(var);
    while(1) {
        sleep(Branch::cycle);
        if(SendStringToMaster(clntSock , (char*)"data")==RET_FAILURE) {
            // pthread_exit(NULL);
        }

    }
}

template <typename T>
static bool CheckEqualVector(std::vector<T> &vec, char *str) {
    for (unsigned int i = 0; i < vec.size(); ++i)
        if(strcmp(str, vec[i].name)==0)
            return true;
    return false;
}

bool SendStringToMaster(int clntSock, char *str) {
    if ( send(clntSock, str, strlen(str), 0) == -1) {
        SEED_WARNING << "Sending to master failed";
        return RET_FAILURE;
    }
    else 
        return RET_SUCCESS;
}

}	// end of namespace TREE