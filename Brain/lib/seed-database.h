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
#include "seed-log.h" // Print information and write to a log-file
using namespace TREE;

#define LOGFILE "seed-database.log"

// There are 3 level of DEBUG_LEVEL from 1->3
// Higher level will debug more information
// Set  DEBUG_LEVEL 0 means : show WARNING, ERROR but no LOG.
// 		DEBUG_LEVEL 1 means : show WARNING, ERROR and LOG.
// 		DEBUG_LEVEL 2 means : show all.
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 1
#else
#undef DEBUG_LEVEL
#define DEBUG_LEVEL 1
#endif

typedef mongoc_collection_t db_collection;

typedef struct {
   int started;		// The total of query command
   int succeeded;	// number of succession
   int failed;		// number of failure
   int missed;		// number of query had not implemented its purpose
} stats_t;

class Database {

public:
	
	Database(const char *DATABASE_NAME);
	~Database();

	bool InsertData(const char *COLL_NAME, char* json);						// insert only one data (document) to a collection
	bool UpdateData(const char *COLL_NAME, char *json, char *jsonSelector); // update only one data to ca collection
	bool DeleteData(const char *COLL_NAME, char *jsonSelector);				// delete only one data
	int64_t TotalDocuments (const char *COLL_NAME);	// count total documents in a specific colletion
	void DisplayTotalQuery();						// Display stats

private:

	mongoc_database_t 	*dtb;	// var for database
	mongoc_client_t     *client;	// var for client connect to database
	const char *dbName;			// the name of database.
	static bool flag_succeed;	// flag for query function know if query is succeed of not. 
	static stats_t stats;			// count the total query command.
	// three funtion below must be static function. (so bad) 
	static void command_started (const mongoc_apm_command_started_t *event);	// show start time of command
	static void command_succeeded (const mongoc_apm_command_succeeded_t *event); // it was called if the query command succeeded
	static void command_failed (const mongoc_apm_command_failed_t *event); // it was called if the query command failed
	static bool TurnFlag(char *s); 	// this function will turn on flag_succeed if "n" : 1 exist in string s and vice versa
									// the issue :  command_succeeded was static function. and so TurnFlag
									// must be static function.
	
};


#endif