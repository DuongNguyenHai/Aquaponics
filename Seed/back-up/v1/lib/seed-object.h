// Nguyen Hai Duong
// Date : july 2016

#ifndef SEED_OBJECT_LEAF
#define SEED_OBJECT_LEAF


#include <stdio.h>
#include <string.h>
#include "seed-database.h"

class Leaf{

private:

	mongoc_collection_t *colt;
	char *json;

	static int totalLeaf; // total number of leaves
	const char *databaseName;
public:

	bool state = true;
	int data;
	int heartTemp;
	char *warning;

	Leaf(const char *DATABASE_NAME, const char *ColtName);
	~Leaf();

	bool InsertLeafToDatabase(char *json);

	void DisplayTotalLeaf();
};


#endif