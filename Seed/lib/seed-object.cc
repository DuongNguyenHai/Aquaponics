#include "seed-object.h"

Leaf::Leaf(const char *DATABASE_NAME, const char *ColtName){
	colt = OpenCollection(DATABASE_NAME, ColtName);
	databaseName = DATABASE_NAME;
}

Leaf::~Leaf(){
	CloseCollection(colt);
}

bool Leaf::InsertLeafToDatabase(char *json){

	InsertData(colt, json);
	return 1;
}