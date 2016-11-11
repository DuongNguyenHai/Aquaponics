// Nguyen Hai Duong
// Date : August 2016

#ifndef SEED_JSON_H
#define SEED_JSON_H

#include <string.h>

// json external [ I WANT WRITE MY OWN JSON LIBRARY ]
// json json-c
#include "base/lib-json/include/json-c/json.h"
// json jsmn
#include "base/lib-json-jsmn/include/jsmn/jsmn.h"
// seed log
#include "seed-log.h"

#define LOG true

#define TP_STRING_STR "char"
#define TP_INT_STR "int"
#define TP_BOOL_STR "bool"
#define TP_DOUBLE_STR "double"

// [1] 	Convert void *values[] to appreciate type (char,int,bool,double) to use
// 		it was created to work with  [2]
// 		+	VOID_STRING(values[i]);
// 		+	...
#define VOID_STRING(ARG) (char *)(ARG)
#define VOID_INT(ARG) *( (int *)ARG)
#define VOID_BOOL(ARG) *( (bool *)ARG)
#define VOID_DOUBLE(ARG) *( (double *)ARG)

// [2] 	marco function for void *values[]
// 		the purpose is to save many diff types in one array : void* values[]
// 		usage :
// 		+	VOID_POINT_CHAR(values[i], data);
// 		+	...
#define VOID_POINT_STRING(ARG,VAL) ({ ARG = VAL; })
#define VOID_POINT_INT(ARG,VAL) ({  ARG = malloc(sizeof(int)); (*( (int *)ARG)) = VAL; })
#define VOID_POINT_BOOL(ARG,VAL) ({  ARG = malloc(sizeof(bool)); (*( (bool *)ARG)) = VAL; })
#define VOID_POINT_DOUBLE(ARG,VAL) ({  ARG = malloc(sizeof(double)); (*( (double *)ARG)) = VAL; })

// [3] marco count number of rows of an array
#define columns_of_array(name)    \
    (sizeof(name[0]) / sizeof(name[0][0]))
#define rows_of_array(name)   \
    ( (sizeof(name) / sizeof(name[0][0])) / columns_of_array(name) )

// [4] marco and struct data for get data from string json
typedef enum {
		TP_BOOL = 1,
		TP_INT = 2,
		TP_DOUBLE = 3,
		TP_STRING = 4,
} sd_enum_type;

typedef struct sd_struct_json {
	sd_enum_type type;
	jsmntype_t hash_type;
	char *name;
	void *val;
	struct sd_struct_json *sub;

} sd_struct_json;

// [] 	API for json

bool JsonValid(char *string);
bool JsonParse(char *string);
char * JsonCreateObject(const char *OBJECT_PROP[][2], long int numRows, void *values[5]);


// get data from string json
struct sd_struct_json *json_parse(char * json_string);

// print sd_struct_json
void Print_sd_struct_json(struct sd_struct_json *json_data);

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