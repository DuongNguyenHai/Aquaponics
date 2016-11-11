// Nguyen Hai Duong
// Date : August 2016

#ifndef SEED_JSON_H
#define SEED_JSON_H

#include <string.h>

// json external [ I WANT WRITE MY OWN JSON LIBRARY ]
// json json-c
#include "cJSON.h"
// seed log
#include "seed-log.h"

namespace TREE {

bool JsonIsValid(char *str);
bool JsonParse(char *str);

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