#include "seed-json.h"

namespace TREE {

// Define a file which is used for write log

// #ifdef LOGFILE
// #undef LOGFILE
// #define LOGFILE "../log/seed-json.log"
// #else
// #define LOGFILE "../log/seed-json.log"
// #endif

bool JsonIsValid(const char *str) {
	
    cJSON *json = cJSON_Parse(str);
    if (!json) {
    	SEED_WARNING << "Error before : " << cJSON_GetErrorPtr();
        cJSON_Delete(json);
        return RET_FAILURE;
    }
    else {
        cJSON_Delete(json);
        return RET_SUCCESS;
    }
}

int JsonGetHeader(const char *str, std::string header[], int sizeHd) {

    cJSON * root = cJSON_Parse(str);
    int index=-1;
    for (int i = 0; i < sizeHd; ++i)
    {
        if(cJSON_GetObjectItem(root,  header[i].c_str())) {
            index = i;
            break;
        }
    }
    cJSON_Delete(root);
    return index;
}

bool JsonHasItem(char *str, const char *item) {

    cJSON * root = cJSON_Parse(str);
    if(cJSON_HasObjectItem(root, item)) {
        cJSON_Delete(root);
        return 1;
    }
    else {
        cJSON_Delete(root);
        return 0;
    }
}

void PrintJson(char *addstr, char *str) {
    cJSON *json = cJSON_Parse(str);
    char *out = cJSON_Print(json);
    SEED_LOG << addstr << out;
    free(out);
}

} // end of namespace TREE