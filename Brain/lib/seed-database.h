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
#include "seed-config.h" // configuration for system
#include "seed-log.h" // Print information and write to a log-file
#include "seed-json.h"

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
	int InsertData(const char *COLL_NAME, char* json);						// insert only one data (document) to a collection
	int InsertDataWithIdAutoIncrement(const char *COLL_NAME, char* json);	// insert a data which has "_id" auto increment
	int UpdateData(const char *COLL_NAME, char *json, char *jsonSelector); // update only one data to ca collection
	int DeleteData(const char *COLL_NAME, char *jsonSelector);				// delete only one data
	int64_t TotalDocuments (const char *COLL_NAME);	// count total documents in a specific colletion
	void DisplayTotalQuery();						// Display stats

private:

	mongoc_database_t 	*dtb;		// var for database
	mongoc_client_t     *client;	// var for client connect to database
	const char *dbName;				// the name of database.
	
};

} // end of namspace TREE


#endif