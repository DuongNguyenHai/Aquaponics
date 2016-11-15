// Nguyen Hai Duong
// Date : Oct 2016

#ifndef SEED_DATABASE_H
#define SEED_DATABASE_H

#include <stdio.h>
#include <string.h>
#include <inttypes.h>	// int64_t
// mongodb c driver
// #include "base/lib-mongo/include/libbson-1.0/bson.h"
// #include "base/lib-mongo/include/libbson-1.0/bcon.h"
// #include "base/lib-mongo/include/libmongoc-1.0/mongoc.h"
#include <mongoc.h>
#include "seed-json.h"
#include "seed-log.h" // Print information and write to a log-file
#include "seed-config.h" // configuration for system

namespace TREE {

typedef mongoc_collection_t db_collection;

typedef struct {
   int started;		// The total of query command
   int succeeded;	// number of succession
   int failed;		// number of failure
   int missed;		// number of query had not implemented its purpose
} stats_t;

class Database {

public:
	
	Database(const char *databaseName);
	~Database();
	bool Check();	// Check the connection to database
	int InsertData(const char *COLL_NAME, char* json);						// insert only one data (document) to a collection
	int InsertDataWithIdAutoIncrement(const char *COLL_NAME, char* json);	// insert a data which has "_id" auto increment
	int UpdateData(const char *COLL_NAME, char *json, char *jsonSelector); // update only one data to ca collection
	int DeleteData(const char *COLL_NAME, char *jsonSelector);				// delete only one data
	int64_t TotalDocuments (const char *COLL_NAME);	// count total documents in a specific colletion
	void DisplayTotalQuery();						// Display stats

private:

	mongoc_database_t 	*dtb;	// var for database
	mongoc_client_t     *client;	// var for client connect to database
	const char *dbName;			// the name of database.
	mongoc_apm_callbacks_t *callbacks;
	// three funtion below must be static function. (so bad) 
	friend void command_started (const mongoc_apm_command_started_t *event);	// show start time of command
	friend void command_succeeded (const mongoc_apm_command_succeeded_t *event); // it was called if the query command succeeded
	friend void command_failed (const mongoc_apm_command_failed_t *event); // it was called if the query command failed
	friend bool TurnFlag(char *s); 	// this function will turn on flag_succeed if "n" : 1 exist in string s and vice versa
									// the issue :  command_succeeded was static function. and so TurnFlag
									// must be static function.
	
};

void command_started (const mongoc_apm_command_started_t *event);
void command_succeeded (const mongoc_apm_command_succeeded_t *event);
void command_failed (const mongoc_apm_command_failed_t *event);
bool TurnFlag(char *s);
} // end of namspace TREE


#endif