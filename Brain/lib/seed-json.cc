#include "seed-json.h"

namespace TREE {

#define LOGFILE "seed-TCP.log" // the file is used for write log
static int DEBUG_LEVEL = 2; // DEBUG_DATABASE_LV was defined in seed-config.cc

bool JsonIsValid(char *str) {
	char *out;
    cJSON *json;

    json = cJSON_Parse(str);
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

} // end of namespace TREE