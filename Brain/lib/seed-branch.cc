// lib/seed-branch.cc
// Nguyen Hai Duong
// Date : Nov 2016

#include "seed-branch.h"

namespace TREE {

#define LOGFILE "seed-branch.log" // the file is used for write log
static int DEBUG_LEVEL = 1; // DEBUG_BRANCH_LV was defined in seed-config.cc

static std::string header[] = {"set","update","data"};
static std::string device[] = {"tempt","DO","amoniac","nitrit","nitrat","hardness"};

static std::vector<Twig> twigs;   // initial a twig (it contains all name of sensor)
// Database dt(DATABASE_NAME);
// Check if str is the same to vec.name
template <typename T>
static bool CheckEqualVector(std::vector<T> &vec, char *str);
void CheckCommand(char *str);
// Printf tiwgs and leaf : name and state
static void PrintTwigs();

Leaf::Leaf() {}
Leaf::~Leaf() {
    SEED_VLOG << "destructor Leaf was called";
    free(name);
}

Twig::Twig() {}
Twig::~Twig() { 
    SEED_VLOG << "destructor Twig was called";
    leaves.clear();
    free(name);
} // must has this function for the case multi class use base class

Branch::Branch() {}
Branch::~Branch() {} // must has this function for the case multi class use base class

void Branch::Start() {
    Workspace::Start(HandleBranch, BRANCH_PORT);
    dt.Check();
}

void HandleBranch(int clntSock, fd_set *set) {

    int recvMsgSize;                    /* Size of received message */
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);

    recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
    if (recvMsgSize < 0) 
        SEED_WARNING << "Error reading from socket";
    else if (recvMsgSize>0){
        SEED_VLOG << ". Branch["<< clntSock-3<<"]: sending"; // abstract 3 for convenience
        CheckCommand(buffer);
        bzero(buffer,strlen(buffer));
    }
    else {
        SEED_VLOG << ". Branch["<< clntSock-3<<"]: disconnected";
        close(clntSock);
        FD_CLR(clntSock, set);
    }     
}

void CheckCommand(char *str) {
    JsonIsValid(str);
    int index = JsonGetHeader(str, header, sizeof(header)/sizeof(header[0]));
    if(index>=0) {
        SEED_VLOG << "The command is : " << header[index].c_str();

        switch(index) {
            case 0 : {
                SetTwigs(str, header[0].c_str());
            } break;
            case 1 : {
                Branch::CheckUpdateTwig(str);           // log any diff
                SetTwigs(str, header[1].c_str());
            } break;
            case 2 : {
                SaveDataToTwig(str);

            } break;
            default: break;
        }
    }
    else
        SEED_WARNING << "Not found command from branch";
}   

bool SetTwigs(char *str, const char *header) {

    twigs.clear();
    Json *root = JsonParse(str);
    Json *jnDevice = JsonGetObjectItem(root, header);  // get item form device
    int sizeJnDevice = JsonArraySize(jnDevice);
    twigs.resize(sizeJnDevice);
    cJSON *item, *subitem;
    // Create twigs
    for (int i = 0 ; i < sizeJnDevice; ++i) {
        item = JsonGetArrayItem(jnDevice, i);
        
        twigs.at(i).state = true;
        twigs.at(i).name = strdup(JsonItemName(item));
        twigs.at(i).leaves.resize(JsonArraySize(item));
        // Create leaves
        for (int j = 0; j < JsonArraySize(item); ++j) {
            subitem = JsonGetArrayItem(item, j);
            twigs.at(i).leaves.at(j).name = strdup(JsonItemName(subitem));
            twigs.at(i).leaves.at(j).state = JsonItemValBool(subitem);
        }
    }
   
    cJSON_Delete(root);
    return RET_SUCCESS;
}

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
                if(CheckEqualVector<Leaf>(twigs.at(i).leaves, JsonItemName(subitem)));

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

template <typename T>
static bool CheckEqualVector(std::vector<T> &vec, char *str) {
    for (unsigned int i = 0; i < vec.size(); ++i)
        if(strcmp(str, vec.at(i).name)==0)
            return true;
    return false;
}

static void PrintTwigs() {

    for (unsigned int i = 0; i < twigs.size(); ++i) {
        SEED_LOG << "twig: " << twigs.at(i).name << ", state: " <<  ((twigs.at(i).state) ? "alive":"die");
        for (unsigned int j = 0; j < twigs.at(i).leaves.size(); ++j)
            SEED_LOG << "\t- leaf: " << twigs.at(i).leaves.at(j).name << ", state: " <<  ((twigs.at(i).leaves.at(j).state) ? "alive":"die");
    }

}

}	// end of namespace TREE