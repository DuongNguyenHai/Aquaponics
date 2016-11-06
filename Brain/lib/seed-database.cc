#include "seed-database.h"

namespace TREE {

bool Database::flag_succeed = false;
stats_t Database::stats = {0};

#define LOGFILE "seed-database.log" // the file is used for write log
static int DEBUG_LEVEL = 0; // DEBUG_DATABASE_LV was defined in seed-config.cc

bool Database::TurnFlag(char *s) {
	if(s[strchr(s,'}')-s-2]-'0')
		flag_succeed = true;
	else
		flag_succeed = false;
	return flag_succeed;
}

void Database::command_started (const mongoc_apm_command_started_t *event) {

   char *s;
  
   s = bson_as_json (mongoc_apm_command_started_get_command (event), NULL);

   SEED_VLOG << "Command " << mongoc_apm_command_started_get_command_name (event)
   			 << " started on " << mongoc_apm_command_started_get_host (event)->host
   			 << ":\n" << s << "\n";

   bson_free (s);
}

void Database::command_succeeded (const mongoc_apm_command_succeeded_t *event) {

   char *s;
  
   s = bson_as_json (mongoc_apm_command_succeeded_get_reply (event), NULL);
   
	if(TurnFlag(s)) {
		SEED_LOG << "Command " << mongoc_apm_command_succeeded_get_command_name (event)
			<< " succeeded:\n" << s << "\n";
	} else {
		SEED_WARNING << "Command " << mongoc_apm_command_succeeded_get_command_name (event)
			<< " missed:\n" << s << "\n";
	}

   bson_free (s);
}

void Database::command_failed (const mongoc_apm_command_failed_t *event) {

   bson_error_t error;
   
   mongoc_apm_command_failed_get_error (event, &error);

   SEED_WARNING <<"Command "<< mongoc_apm_command_failed_get_command_name (event)
   				<< " failed:" << error.message << "\n";
}

Database::Database(const char *DATABASE_NAME) {

	DEBUG_LEVEL = DEBUG_DATABASE_LV; // set debug level for database

	bson_t *command, reply;
	bson_error_t error;
	mongoc_apm_callbacks_t *callbacks;

	mongoc_init ();
	// Create a new client
	client = mongoc_client_new ("mongodb://127.0.0.1/");

	mongoc_client_set_error_api (client, 2);
   	callbacks = mongoc_apm_callbacks_new ();

	mongoc_apm_set_command_started_cb (callbacks, command_started);
	mongoc_apm_set_command_succeeded_cb (callbacks, command_succeeded );
	mongoc_apm_set_command_failed_cb (callbacks, command_failed);
	mongoc_client_set_apm_callbacks (client, callbacks, NULL /* context pointer */);

	dbName = DATABASE_NAME;

	dtb = mongoc_client_get_database (client, dbName);

	command = BCON_NEW ("ping", BCON_INT32 (1));

	if (!mongoc_client_command_simple (client, "admin", command, NULL, &reply, &error)) {
		bson_destroy (command);
		SEED_ERROR << error.message;
		exit(RET_FAILURE);
	}

	bson_destroy (command);
	SEED_LOG << "Database connection succeeded !";
}

Database::~Database() {
	mongoc_database_destroy (dtb);
	mongoc_client_destroy (client);
	mongoc_cleanup ();
}

// char *json = (char *)"{\"temp\": 25, \"heartTemp\":35, \"state\":1, \"warning\": \"none\" }";
// object.InsertData("collection name", json);
int Database::InsertData(const char *COLL_NAME, char* json) {
	
	bson_t *command;
	bson_error_t error;
	db_collection *colt = mongoc_client_get_collection (client, dbName, COLL_NAME);

	stats.started++;

	if( !(command = bson_new_from_json ((const uint8_t*)json, -1, &error))) {
		goto JUMP_FAIL;
	}

	if( !mongoc_collection_insert (colt, MONGOC_INSERT_NONE, command, NULL, &error)) {
		bson_destroy (command);
		goto JUMP_FAIL;
	}

	bson_destroy (command);
	mongoc_collection_destroy (colt);

	if(flag_succeed)
		stats.succeeded++;
	else {
		stats.missed++;
		return RET_MISS;
	}

	return RET_SUCCESS;

	JUMP_FAIL:
		stats.failed++;
		mongoc_collection_destroy (colt);
		SEED_ERROR << error.message;
		return RET_FAILURE;

}

// char *jsonSelector = (char *)"{\"temp\":25}";
// char *jsonUpdate = (char *)"{\"$set\":{\"heartTemp\":100}}";
// object.UpdateData("collection name", jsonUpdate, jsonSelector);
int Database::UpdateData(const char *COLL_NAME, char *json, char *jsonSelector) {

	bson_t *command, *query;
	bson_error_t error;
	db_collection *colt = mongoc_client_get_collection (client, dbName, COLL_NAME);

	stats.started++;

	if(!(command = bson_new_from_json ((const uint8_t*)json, -1, &error))) {
		goto JUMP_FAIL;
	}

	if(!(query = bson_new_from_json ((const uint8_t*)jsonSelector, -1, &error))) {
		bson_destroy (command);
		goto JUMP_FAIL;
	}

	if( !mongoc_collection_update (colt, MONGOC_UPDATE_NONE, query, command, NULL, &error)) {	
		stats.failed++;
		bson_destroy (command);
		bson_destroy (query);
		goto JUMP_FAIL;
	}

	bson_destroy (command);
	bson_destroy (query);
    mongoc_collection_destroy (colt);

	if(flag_succeed)
		stats.succeeded++;
	else {
		stats.missed++;
		return RET_MISS;
	}

	return RET_SUCCESS;

	JUMP_FAIL:
		stats.failed++;
		mongoc_collection_destroy (colt);
		SEED_ERROR << error.message;
		return RET_FAILURE;
}

// char *jsonSelector = (char *)"{\"temp\":25}";
// object.DeleteData("collection name", jsonUpdate, jsonSelector);
int Database::DeleteData(const char *COLL_NAME, char *jsonSelector){

	bson_t *query;
	bson_error_t error;
	db_collection *colt = mongoc_client_get_collection (client, dbName, COLL_NAME);

	stats.started++;

	if(!(query = bson_new_from_json ((const uint8_t*)jsonSelector, -1, &error))) {
		goto JUMP_FAIL;
	}

	if( !mongoc_collection_remove (colt, MONGOC_REMOVE_SINGLE_REMOVE, query, NULL, &error)) {
		bson_destroy (query);
	 	goto JUMP_FAIL;
	}

	bson_destroy (query);
    mongoc_collection_destroy (colt);

	if(flag_succeed)
		stats.succeeded++;
	else {
		stats.missed++;
		return RET_MISS;
	}

    return RET_SUCCESS;

    JUMP_FAIL:
		stats.failed++;
		mongoc_collection_destroy (colt);
		SEED_ERROR << error.message;
		return RET_FAILURE;
}

// object.TotalDocuments("collection name");
int64_t Database::TotalDocuments (const char *COLL_NAME){

   	int64_t count;
   	bson_error_t error;
   	db_collection *colt = mongoc_client_get_collection (client, dbName, COLL_NAME);

	if ( (count = mongoc_collection_count (colt, MONGOC_QUERY_NONE, NULL, 0, 0, NULL, &error)) < 0) {
		mongoc_collection_destroy (colt);
		SEED_ERROR << error.message;
		return RET_FAILURE;
	} else {
		mongoc_collection_destroy (colt);
		printf ("The \"%s\" collection has : %" PRId64 " documents.\n", COLL_NAME, count);
		return count;
	}
}

void Database::DisplayTotalQuery() {
	
	SEED_LOG << "Total query: " << stats.started 
			 << ", succeeded: " << stats.succeeded
			 << ", failed: " << stats.failed
			 << ", missed: " << stats.missed << "\n";
}

} // end of namespace TREE

