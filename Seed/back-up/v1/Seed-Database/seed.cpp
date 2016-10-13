// Nguyen Hai Duong
// Date : july 2016

#include <stdio.h>      // for printf() and fprintf()
#include <string.h>
#include "../lib/seed-database.h"

// using namespace seed;
#define DATABASE_NAME "duong"
#define sensor "sensor"
#define PH "PH"

char insert[] = "{\"temp\": 25, \"heartTemp\":35, \"state\":1, \"warning\": \"none\" }";
char *json = insert;


int main (int   argc, char *argv[]){

	mongoc_database_t *database = ConnectDatabase(DATABASE_NAME);

	mongoc_collection_t *colt = OpenCollection(DATABASE_NAME,sensor);

	InsertData(colt,json);
	TotalDocuments(colt);

	DisconnectDatabase(database);

	return 0;
}