// Nguyen Hai Duong
// Date : August 2016

#ifndef SEED_JSON_H
#define SEED_JSON_H

#include <iostream>
#include <string.h>

// json external [ I WANT WRITE MY OWN JSON LIBRARY ]
// json json-c
#include "cJSON.h"
// seed log
#include "seed-log.h"

namespace TREE {

typedef cJSON Json;
#define JsonParse(str) cJSON_Parse(str)
#define JsonGetItem(root, str) cJSON_GetObjectItem(root, str)
#define JsonArraySize(obj) cJSON_GetArraySize(obj)
#define JsonGetArrayItem(obj, i) cJSON_GetArrayItem(obj, i)
#define JsonItemVal(subitem) (subitem->string)

bool JsonIsValid(char *str);
// bool JsonParse(char *str);

bool JsonHasItem(char *str, const char *check);
int JsonGetHeader(char *str, std::string header[], int sizeHd);
int JsonGetDevice(char *str, std::string device[], int sizeDv);

} // end of namspace TREE


#endif

/*

test API:

-	test vaild json : pass
-	test invalid json : pass
-	test create a new json : pass
-	test get data from string json : fail when there are many tests

test bug:

-	test other type in OBJECT_PROP[][2] : pass

*/