#include "seed-database.h"

static mongoc_client_t      	*client;
static bson_error_t          	error;

mongoc_database_t *ConnectDatabase(const char *DATABASE_NAME){
	bson_t *command, reply;

	mongoc_init ();
	// Create a new client instance
	client = mongoc_client_new ("mongodb://127.0.0.1/");
	// Get a handle on the database "db_name" and collection "coll_name"
	mongoc_database_t *database = mongoc_client_get_database (client, DATABASE_NAME);

	command = BCON_NEW ("ping", BCON_INT32 (1));

	if (!mongoc_client_command_simple (client, "admin", command, NULL, &reply, &error)) {
		sd_bug(error.message);
		return 0;
	}

	#if LOG
	sd_log(">>>> Database connection succeeded !");
	#endif

	return database;
}

mongoc_collection_t *OpenCollection(const char *DATABASE_NAME, const char *coltName){

	mongoc_collection_t *colt = mongoc_client_get_collection (client, DATABASE_NAME, coltName);
	return colt;
}

bool OpenCollection2(mongoc_collection_t **colt, const char *DATABASE_NAME, const char *coltName){

	*colt = mongoc_client_get_collection (client, DATABASE_NAME, coltName);
	return 1;
}

bool InsertData(mongoc_collection_t *colt, char* json){
	
	bson_t *command;

	if( !(command = bson_new_from_json ((const uint8_t*)json, -1, &error)) ){
		sd_bug(error.message);
      	return 0;
	}

	#if LOG
	char *str = bson_as_json (command, NULL);
	sd_log("Insert data : ",NO_NEW_LINE);
	sd_log(str);
	#endif

	if (!mongoc_collection_insert (colt, MONGOC_INSERT_NONE, command, NULL, &error)) {
		sd_bug(error.message);
		return 0;
	}
	return 1;
	

}

bool UpdateData(mongoc_collection_t *colt, char *json, char *jsonQuery){

	bson_t *command, *query;
	if( !(query = bson_new_from_json ((const uint8_t*)jsonQuery, -1, &error)) ){
		sd_bug(error.message);
		return 0;
	}

	if( !(command = bson_new_from_json ((const uint8_t*)json, -1, &error)) ){
		sd_bug(error.message);
		return 0;
	}

	#if LOG
	char *str = bson_as_json (query, NULL);
	sd_log("Update data : ",NO_NEW_LINE);
	sd_log (str,NO_NEW_LINE);
	str = bson_as_json (command, NULL);
	sd_log (str);
	#endif

    if (!mongoc_collection_update (colt, MONGOC_UPDATE_NONE, query, command, NULL, &error)) {
        sd_bug(error.message);
        return 0;
    }

	return 1;
}

bool DeleteData(mongoc_collection_t *colt, char *jsonQuery){

	bson_t *query;

	if( !(query = bson_new_from_json ((const uint8_t*)jsonQuery, -1, &error)) ){
		sd_bug(error.message);
		return 0;
	}

	#if LOG
	char *str = bson_as_json (query, NULL);
	sd_log("Delete data : ",NO_NEW_LINE);
	sd_log (str);
	#endif

    if (!mongoc_collection_remove (colt, MONGOC_REMOVE_SINGLE_REMOVE, query, NULL, &error)) {
        sd_bug(error.message);
        return 0;
    }
    return 0;

}

void DisconnectDatabase(mongoc_database_t *database){

	mongoc_database_destroy (database);
	mongoc_client_destroy (client);
	mongoc_cleanup ();
}

void CloseCollection(mongoc_collection_t *colt){
	mongoc_collection_destroy (colt);
}

int64_t TotalDocuments (mongoc_collection_t *colt){

   	int64_t count;

	if ( (count = mongoc_collection_count (colt, MONGOC_QUERY_NONE, NULL, 0, 0, NULL, &error)) < 0) {
		sd_bug(error.message);
		return 0;
	} else {
		
		#if LOG
		printf ("%"PRId64" documents counted.\n", count);
		#endif
		return count;
	}

   
}
