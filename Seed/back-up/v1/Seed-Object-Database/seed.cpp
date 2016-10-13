// Nguyen Hai Duong
// Date : july 2016

#include <stdio.h>      // for printf() and fprintf()
#include <string.h>
#include "../lib/seed-database.h"
#include "../lib/seed-object.h"
// using namespace seed;
#define DATABASE_NAME "duong"
// #define sensor "sensor"
// #define motor "motor"

char insert[] = "{\"temp\": 25, \"heartTemp\":35, \"state\":1, \"warning\": \"none\" }";
char *json = insert;


int main (int   argc, char *argv[]){

	mongoc_database_t *database = ConnectDatabase(DATABASE_NAME);

	Leaf sensor(DATABASE_NAME,"sensor");
	Leaf PH(DATABASE_NAME,"PH");

	sensor.InsertLeafToDatabase(json);
	PH.InsertLeafToDatabase(json);
	DisconnectDatabase(database);

	return 0;
}