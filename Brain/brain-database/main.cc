// Nguyen Hai Duong
// Date : july 2016

#include <stdio.h>      // for printf() and fprintf()
#include <string.h>
#include "../lib/seed-database.h"

using namespace TREE;

#define DATABASE_NAME "duong"
#define sensor "sensor"

char insert[] = "{\"temp\": 25, \"heartTemp\":35, \"state\":1, \"warning\": \"none\" }";
char *json = insert;


char *jsonSelector = (char *)"{\"heartTemp\":35}";
char *jsonUpdate = (char *)"{\"$set\":{\"heartTemp\":100}}";
char *jsonSelector2 = (char *)"{\"temp\":25}";

int main (int   argc, char *argv[]) {

	Database dt(DATABASE_NAME);

	dt.InsertData(sensor, json);
	dt.UpdateData(sensor, jsonUpdate, jsonSelector);
	dt.DeleteData(sensor, jsonSelector2);
	dt.TotalDocuments(sensor);
	dt.DisplayTotalQuery();
	
	return 0;
}