#include "seed-json.h"

namespace TREE {

#define LOGFILE "seed-TCP.log" // the file is used for write log
static int DEBUG_LEVEL = 2; // DEBUG_DATABASE_LV was defined in seed-config.cc

bool JsonIsValid(char *str) {
	char *out;

    cJSON *json = cJSON_Parse(str);
    if (!json) {
    	SEED_WARNING << "Error before : " << cJSON_GetErrorPtr();
        return RET_FAILURE;
    }
    else {
        out = cJSON_Print(json);
        cJSON_Delete(json);
        SEED_VLOG << out;
        free(out);
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