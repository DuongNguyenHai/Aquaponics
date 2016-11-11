#include "seed-json.h" // Print information and write to a log-file
#include <stdio.h>
// using namespace TREE;

// There are 3 level of DEBUG_LEVEL from 1->3
// Higher level will debug more information
// Set  DEBUG_LEVEL 0 means : show WARNING, ERROR but no LOG.
// 		DEBUG_LEVEL 1 means : show WARNING, ERROR and LOG.
// 		DEBUG_LEVEL 2 means : show all.

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 2
#else
#undef DEBUG_LEVEL
#define DEBUG_LEVEL 2
#endif

#define LOGFILE "main.log"

/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
    char *out;
    cJSON *json;

    json = cJSON_Parse(text);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
        out = cJSON_Print(json);
        cJSON_Delete(json);
        printf("%s\n", out);
        free(out);
    }
}

int main(int argc, char const *argv[])
{
	char * string = (char*)"{\"name\" : \"joys of programming\",\"what\":123}";
	char text1[] =
        "{\n"
        "\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n"
        "\"format\": {\"type\":       \"rect\", \n"
        "\"width\":      1920, \n"
        "\"height\":     1080, \n"
        "\"interlace\":  false,\"frame rate\": 24\n"
        "}\n"
        "}";
	/* Process each json textblock by parsing, then rebuilding: */
    doit(string);
    doit(text1);
	return 0;
}