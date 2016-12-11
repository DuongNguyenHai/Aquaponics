// Nguyen Hai Duong
// Date : August 2016

#ifndef SEED_JSON_H
#define SEED_JSON_H

#include <iostream>
#include <string.h>
#include "cJSON.h"
#include "seed-log.h"

namespace TREE {

// Rededine marco from cJSON.h
typedef cJSON Json;
#define JsonParse(str) cJSON_Parse(str)
#define JsonGetObjectItem(root, str) cJSON_GetObjectItem(root, str)
#define JsonArraySize(arry) cJSON_GetArraySize(arry)
#define JsonGetArrayItem(arry, i) cJSON_GetArrayItem(arry, i)
#define JsonItemName(subitem) (subitem->string)
#define JsonItemValBool(subitem) (subitem->valueint)
#define JsonItemValInt(subitem) (subitem->valueint)
#define JsonItemValStr(subitem) (subitem->valuestring)
#define JsonItemDouble(subitem) (subitem->valuedouble)
#define JsonDetachItemFromObject(object, string) cJSON_DetachItemFromObject(object, string)
#define JsonInsertItemInArray(array, which, newitem) cJSON_InsertItemInArray(array, which, newitem)
#define JsonAddNumberToObject(object, name, n) cJSON_AddNumberToObject(object,name,n)
#define	JsonDelete(root) cJSON_Delete(root)
#define JsonAddNumberToObject(object,name,n) cJSON_AddNumberToObject(object,name,n)
#define JsonAddStringToObject(object,name,s) cJSON_AddStringToObject(object,name,s)
bool JsonIsValid(const char *str); // check string json is valid or not.
bool JsonHasItem(char *str, const char *item);	// check whether item is exist in str
int JsonGetHeader(const char *str, std::string header[], int sizeHd);	// Get a header of string and checking whether it is exist on header[] list, if it exist, return the index of header in header[] else return -1
void PrintJson(char *addstr = NULL, char *str = NULL); // Print json
} // end of namspace TREE


#endif

/*

test API:

-	JsonIsValid : passed
-	JsonHasItem : passed
-	JsonGetHeader : passed

test bug:

*/