#include "seed-object.h"

// char insert[] = "{\"temp\": 25, \"heartTemp\":35, \"state\":1, \"warning\": \"none\" }";
char update[] = "{\"$set\":{\"temp\":1000}}";
char query[] = "{\"_id\":1}";
char *jsonUpdate = update;
char *jsonQuery = query;

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