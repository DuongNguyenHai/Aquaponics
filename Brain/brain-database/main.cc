// Nguyen Hai Duong
// Date : july 2016

#include <stdio.h>      // for printf() and fprintf()
#include <string.h>
#include "seed-config.h"
#include "seed-database.h"
#include "seed-parse-config.h"

using namespace TREE;

#define SENSOR "sensor"

char insert[] = "{\"temp\": 25, \"heartTemp\":35, \"state\":1, \"warning\": \"none\" }";
char *json = insert;


char *jsonSelector = (char *)"{\"heartTemp\":35}";
char *jsonUpdate = (char *)"{\"$set\":{\"heartTemp\":100}}";
char *jsonSelector2 = (char *)"{\"temp\":25}";

int main (int   argc, char *argv[]) {

	ParseOptions po("../config/sys-config.cfg");
	Database dt(DATABASE_NAME);

	dt.InsertData(SENSOR, json);
	dt.UpdateData(SENSOR, jsonUpdate, jsonSelector);
	dt.DeleteData(SENSOR, jsonSelector2);
	dt.TotalDocuments(SENSOR);
	dt.DisplayTotalQuery();
	
	return 0;
}