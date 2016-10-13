// Nguyen Hai Duong
// Date : july 2016

#ifndef SEED_DATABASE_H
#define SEED_DATABASE_H

#include <stdio.h>
#include <string.h>
#include <inttypes.h>	// int64_t

// mongodb c driver
#include "base/lib-mongo/include/libbson-1.0/bson.h"
#include "base/lib-mongo/include/libbson-1.0/bcon.h"
#include "base/lib-mongo/include/libmongoc-1.0/mongoc.h"

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

// Disconect to Database
void DisconnectDatabase(mongoc_database_t *database);


// The alternative way of OpenCollection : OpenCollection2(&colt,DATABASE_NAME, sensor); // mongoc_collection_t *colt
bool OpenCollection2(mongoc_collection_t **colt,const char *DATABASE_NAME, const char *coltName);

#endif