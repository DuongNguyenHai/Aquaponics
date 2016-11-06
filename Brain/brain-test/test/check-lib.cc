// Nguyen Hai Duong
// Date : july 2016

#include <stdio.h>      // for printf() and fprintf()
#include <string.h>
#include "seed-database.h"

using namespace TREE;

#define DATABASE_NAME "duong"
#define SENSOR "sensor"

char insert[] = "{\"temp\": 25, \"heartTemp\":35, \"state\":1, \"warning\": \"none\" }";
char *json = insert;


char *jsonSelector = (char *)"{\"heartTemp\":35}";
char *jsonUpdate = (char *)"{\"$set\":{\"heartTemp\":100}}";
char *jsonSelector2 = (char *)"{\"temp\":25}";

char *jsonSeltWr = (char *)"{\"heartTemp\":1234}";

int main (int   argc, char *argv[]) {

	Database dt(DATABASE_NAME);
	bool flag = true;

	// Check insert query
	if( dt.InsertData(SENSOR, json) == RET_FAILURE ) {
		printf("[Insert query failed]\n");
		flag = false;
	}

	// Check upadte query
	if( dt.UpdateData(SENSOR, jsonUpdate, jsonSelector) == RET_FAILURE ) {
		printf("[Update query failed]\n");
		flag = false;
	}

	// Check delete query
	if( dt.DeleteData(SENSOR, jsonSelector2) == RET_FAILURE ) {
		printf("[Delete query failed]\n");
		flag = false;
	}

	if(flag)
		printf("[...OK...]\n");

	printf("\nCheck Update query was missed :\n");
	if( dt.UpdateData(SENSOR, jsonUpdate, jsonSeltWr) == RET_MISS )
		printf("[...OK...]\n");
	else{
		printf("[Update query was not return RET_MISS function fail]!\n");
		flag = false;
	}

	printf("\nCheck Delete query was missed :\n");
	if( dt.DeleteData(SENSOR, jsonSeltWr) == RET_MISS )
		printf("[...OK...]\n");
	else{
		printf("[Delte query was not return RET_MISS function fail]!\n");
		flag = false;
	}
	printf("\nCheck counter query:\n");
	// Check counter : count total documents in a collection
	if( dt.TotalDocuments(SENSOR)>=0 ) 
		printf("[...OK...]\n");
	else {
		printf("[ Function count documents failed! ]\n");
		flag = false;
	}

	dt.DisplayTotalQuery();

	if (flag)
		return RET_SUCCESS;
	else
		return RET_FAILURE;
}