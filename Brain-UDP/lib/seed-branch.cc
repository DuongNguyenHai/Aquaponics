// lib/seed-branch.cc
// Nguyen Hai Duong
// Date : Nov 2016

#include "seed-branch.h"

namespace TREE {

// define the file is used for write log

// #ifdef LOGFILE
// #undef LOGFILE
// #define LOGFILE "../log/seed-branch.log"
// #else
// #define LOGFILE "../log/seed-branch.log"
// #endif

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
static bool SendStringToMaster(int clntSock, struct sockaddr_in serv_addr, char *str);

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
    // Workspace::CreateANewOnlineSpace(HandleBranch, BRANCH_PORT, RequestDataFromMaster);
    Workspace::CreateAPairOfPine(1);    // pipe() must be created before create processes.  so this function have to call firstly
    Workspace::CreateANewOnlineSpace(CreateAConnectionToMaster, StringToChar(MASTER_IP), BRANCH_PORT, RequestDataFromMasterUDP);
    
}

void Branch::CreateAConnectionToMaster(int clntSock) {

    int recvMsgSize;
    char buffer[BUFFSIZE];
    struct sockaddr_in cli_addr;
    unsigned int clntLen = sizeof(cli_addr);
    getpeername(clntSock, (struct sockaddr *) &cli_addr, &clntLen);

    while(1) {
        recvMsgSize = recvfrom(clntSock, buffer, BUFFSIZE, 0,NULL, NULL);
        if (recvMsgSize==-1) {
            SEED_WARNING << "Error reading from socket: " << strerror (errno);
            int data = RQ_FAIL;
            if (write(fd[REQUEST_WRITE], &data, sizeof(data))==-1) {
                SEED_WARNING << "Pipe: Failed to send value to RequestDataFromMasterUDP(). " << strerror (errno);
            }
        }
        else if (recvMsgSize>0) {
            SEED_VLOG << "Branch["<<inet_ntoa(cli_addr.sin_addr)<<"] has sended message"; // abstract 3 for convenience
            bool state = Branch::HandleCommand(clntSock, buffer);
            int data;
            if(state==RET_SUCCESS)
                data = RQ_SUCCESS;
            else 
                data = RQ_FAIL;

            if (write(fd[REQUEST_WRITE], &data, sizeof(data))==-1) {
                SEED_WARNING << "Pipe: Failed to send value to RequestDataFromMasterUDP()";
            }
            bzero(buffer,strlen(buffer));   // need bzero to clear buffer. if dont do that, the next time will print some old stuffs at the end. Still dont know why
        }
        else if(recvMsgSize==0) {
            SEED_LOG << "Branch["<<inet_ntoa(cli_addr.sin_addr)<<"] : recvMsgSize = 0";
            // close(clntSock);
            int data = RQ_FAIL;
            if (write(fd[REQUEST_WRITE], &data, sizeof(data))==-1) {
                SEED_WARNING << "Pipe: Failed to send value to RequestDataFromMasterUDP()";
            }
        }
    }
}

void Branch::HandleBranch(int clntSock, fd_set *set) {

    int recvMsgSize;                    /* Size of received message */
    char buffer[BUFFSIZE];
    struct sockaddr_in cli_addr;
    unsigned int clntLen = sizeof(cli_addr);
    getpeername(clntSock, (struct sockaddr *) &cli_addr, &clntLen);

    recvMsgSize = recv(clntSock, buffer, BUFFSIZE, 0);
    if (recvMsgSize==-1) {
        SEED_WARNING << "Error reading from socket: " << strerror (errno);
        sleep(5);
        SendStringToMaster(clntSock , RQ_DATA);
    }
    else if (recvMsgSize>0) {
        SEED_VLOG << "Branch["<<inet_ntoa(cli_addr.sin_addr)<<"] has sended message"; // abstract 3 for convenience
        Branch::HandleCommand(clntSock, buffer);
        bzero(buffer,strlen(buffer));   // need bzero to clear buffer. if dont do that, the next time will print some old stuffs at the end. Still dont know why
    }
    else if(recvMsgSize==0) {
        SEED_LOG << "Branch["<<inet_ntoa(cli_addr.sin_addr)<<"] has disconnected";
        sleep(5);
        SendStringToMaster(clntSock , RQ_DATA);
        // close(clntSock);
        // Workspace::ClearThread(clntSock);
        // FD_CLR(clntSock, set);
    }

}

bool Branch::HandleCommand(int clntSock, char *str) {

    PrintJson((char *)"Branch's message: \n", str);
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
    Json *item, *subitem;
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
    
    JsonDelete(root);
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

    JsonDelete(root);
    return RET_SUCCESS;
}

// save data to twig (collection)
bool Branch::SaveDataToTwig(char *str) {

    Json *root = JsonParse(str);
    Json *data = JsonGetObjectItem(root, header[2].c_str()); // get item form data
    Json *leaf = JsonGetArrayItem(data, 0);
    
    char *out = cJSON_Print(leaf);
    dt.InsertDataWithIdAutoIncrement(JsonItemName(leaf), out);
    JsonDelete(root);
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

void *Branch::RequestDataFromMaster(void *var) {

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    int clntSock = VOID_TO_INT(var);
    if(INTERVAL_REQUEST>0) {
        while(1) {
            SEED_LOG << "Request data after: " << INTERVAL_REQUEST << "s";
            sleep(INTERVAL_REQUEST);
            if(SendStringToMaster(clntSock , RQ_DATA)==RET_FAILURE) {
                pthread_exit(NULL);
            }
        }
    } else {
        time_t rawtime;
        struct tm * tnow;
        while(1) {
            for (unsigned int i = 0; i < SCHEDULE.size(); ++i) {
                time ( &rawtime );
                tnow = localtime ( &rawtime );
                int timer = SCHEDULE[i] - ( tnow->tm_hour*3600 + tnow->tm_min*60 + tnow->tm_sec );
                if(timer<0)
                    timer = timer + 24*3600;
                char *date = DateAndTime(rawtime+timer);
                SEED_LOG << "Next request data at \""<< date << "\"";
                free(date);
                sleep(timer);

                if(SendStringToMaster(clntSock , RQ_DATA)==RET_FAILURE) {
                    pthread_exit(NULL);
                }
            }
        }
    }
}

void Branch::RequestDataFromMasterUDP(int clntSock, struct sockaddr_in serv_addr) {

    if(INTERVAL_REQUEST>0) {
        
        while(1) {
            SEED_LOG << "Request data after: " << INTERVAL_REQUEST << "s";
            sleep(INTERVAL_REQUEST);
            SendStringToMaster(clntSock, serv_addr, RQ_DATA);
            CheckRequestIsSuccess(clntSock, serv_addr);
        }
    } else {
        time_t rawtime;
        struct tm * tnow;
        while(1) {
            for (unsigned int i = 0; i < SCHEDULE.size(); ++i) {
                time ( &rawtime );
                tnow = localtime ( &rawtime );
                int timer = SCHEDULE[i] - ( tnow->tm_hour*3600 + tnow->tm_min*60 + tnow->tm_sec );
                if(timer<0)
                    timer = timer + 24*3600;
                char *date = DateAndTime(rawtime+timer);
                SEED_LOG << "Next request data at \""<< date << "\"";
                free(date);
                sleep(timer);
                SendStringToMaster(clntSock, serv_addr, RQ_DATA);
                CheckRequestIsSuccess(clntSock, serv_addr);
            }
        }
    }
}

bool Branch::CheckRequestIsSuccess(int clntSock, struct sockaddr_in serv_addr) {
    int data;
    fd_set fds;
    struct timeval tv = { 0 };
    int countTimes = 0;             // count request times
    tv.tv_sec = TIME_OUT_READ_PIPE; // timout for watting read pipe
    FD_ZERO(&fds);                  // zero-out the file descriptor set    

    while(1) {

        FD_SET(fd[BRANCH_READ], &fds); // set "read file descriptor"

        int rv = select(fd[BRANCH_READ] + 1, &fds, NULL, NULL, &tv);

        if(rv==-1) {
            SEED_WARNING << "select() failed: " << strerror(errno);
        } 
        else if(rv==0) {
            countTimes++;
            tv.tv_sec = TIME_OUT_READ_PIPE; // timout for watting read pipe
            
            SendStringToMaster(clntSock, serv_addr, RQ_DATA);
            if((countTimes%60)==0) // time = 60*TIME_OUT_READ_PIPE (s). (default : time = 60*10 = 600s)
                SEED_WARNING << "There was no respond from master after " << countTimes 
                             <<" times request. Perhaps, Connection has lost. Lets check all connection : Root <--> Router <--> Master";
            if((countTimes%180)==0)  // time = 180*TIME_OUT_READ_PIPE (s) (default : time = 180*10 = 1800s)
                SEED_ERROR << "There has no responded from master for a long time. The system will be terminated automatically right now."
                           <<" U should check all connection before turn system on !";
        }
        else {
            int recv = read(fd[BRANCH_READ], &data, sizeof(data));
            if(recv==-1) {
                SEED_WARNING << "Pipe error reading socket: " << strerror (errno);
            } 
            else if(recv==0){
                SEED_WARNING << "Pipe from CreateAConnectionToMaster() was closed";
            } 
            else {
                countTimes=0;
                if(data==RQ_SUCCESS)
                    return RET_SUCCESS;
                else
                    SendStringToMaster(clntSock, serv_addr, RQ_DATA);
            }
        }
        FD_CLR(fd[BRANCH_READ], &fds);
    }

    return RET_SUCCESS;
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

bool SendStringToMaster(int clntSock, struct sockaddr_in serv_addr, char *str) {

    if ( sendto(clntSock, str, strlen(str), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        SEED_WARNING << "Sending to master failed";
        return RET_FAILURE;
    }
    else 
        return RET_SUCCESS;
}

}	// end of namespace TREE