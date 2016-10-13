#ifndef SEED_DATABASE
#define SEED_DATABASE

#include <stdio.h>
#include <string.h>
#include <inttypes.h>	// int64_t

// mongodb c driver
#include <bson.h>
#include <bcon.h>
#include <mongoc.h>

// seed log
#include "seed-log.h"

#define LOG true

mongoc_database_t *ConnectDatabase(const char *DATABASE_NAME);
// Open a collection
// mongoc_collection_t *coll = OpenCollection();
mongoc_collection_t *OpenCollection(const char *DATABASE_NAME, const char *coltName);

// query
bool InsertData(mongoc_collection_t *colt,char *json);
bool UpdateData(mongoc_collection_t *colt,char *json, char *jsonQuery);
bool DeleteData(mongoc_collection_t *colt,char *jsonQuery);
int64_t TotalDocuments (mongoc_collection_t *colt);

// Close collection
void CloseCollection(mongoc_collection_t *colt);
// Close connection
void DisconnectDatabase(mongoc_database_t *database);


// The alternative way of OpenCollection : OpenCollection2(&colt,DATABASE_NAME, sensor); // mongoc_collection_t *colt
bool OpenCollection2(mongoc_collection_t **colt,const char *DATABASE_NAME, const char *coltName);

#endif