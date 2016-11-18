#include "seed-json.h"

namespace TREE {

// Define a file which is used for write log

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE "seed-json.log"
#else
#define LOGFILE "seed-json.log"
#endif

bool JsonIsValid(char *str) {
	
    cJSON *json = cJSON_Parse(str);
    if (!json) {
    	SEED_WARNING << "Error before : " << cJSON_GetErrorPtr();
        cJSON_Delete(json);
        return RET_FAILURE;
    }
    else {
        char *out = cJSON_Print(json);
        SEED_LOG << "Branch's message : \n" << out;
        free(out);
        cJSON_Delete(json);
        return RET_SUCCESS;
    }
}

int JsonGetHeader(char *str, std::string header[], int sizeHd) {

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


} // end of namespace TREE