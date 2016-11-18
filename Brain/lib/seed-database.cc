#include "seed-database.h"

namespace TREE {

static bool flag_succeed= false;	// flag for query function know if query is succeed of not. 
static stats_t stats= {0};			// count the total query command.

// Define a file which  is used for write log

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE "seed-database.log"
#else
#define LOGFILE "seed-database.log"
#endif

// static void command_started (const mongoc_apm_command_started_t *event);	// show start time of command
static void command_excuted (const mongoc_apm_command_succeeded_t *event); // it was called if the query command succeeded
static void command_failed (const mongoc_apm_command_failed_t *event); // it was called if the query command failed
static bool TurnFlag(char *s); 		// this function will turn on flag_succeed if "n"/"ok" : 1 exist in string s and 
									// if "n"/"ok" : 0 will turn off flag_succeed
									// the issue :  command_excuted was static function. and so TurnFlag
									// must be static function.

static bool TurnFlag(char *s) {
	if(s[strrchr(s,'}')-s-2]-'0')	// get value of "ok" : 1
		flag_succeed = true;
	else
		flag_succeed = false;
	return flag_succeed;
}

// static void command_started (const mongoc_apm_command_started_t *event) {
//    char *s;
//    s = bson_as_json (mongoc_apm_command_started_get_command (event), NULL);
//    SEED_VLOG << "Command " << mongoc_apm_command_started_get_command_name (event)
//    			 << " started on " << mongoc_apm_command_started_get_host (event)->host
//    			 << ":\n" << s << "\n";
//    bson_free (s);
// }

static void command_excuted (const mongoc_apm_command_succeeded_t *event) {
	char *s;
	char *e = strdup(mongoc_apm_command_succeeded_get_command_name (event));
	s = bson_as_json (mongoc_apm_command_succeeded_get_reply (event), NULL);
	if(TurnFlag(s)) {

		if( strcmp(e,"insert")==0 || strcmp(e,"update")==0 || strcmp(e,"delete")==0 )
			SEED_LOG << "Command " << mongoc_apm_command_succeeded_get_command_name (event)
				<< " succeeded:\n" << s << "\n";
		else
			SEED_VLOG << "Command " << mongoc_apm_command_succeeded_get_command_name (event)
				<< " succeeded:\n" << s << "\n";
	} else {
		SEED_WARNING << "Command " << mongoc_apm_command_succeeded_get_command_name (event)
			<< " missed:\n" << s << "\n";
	}
   bson_free (s);
   free(e);
}

static void command_failed (const mongoc_apm_command_failed_t *event) {

   bson_error_t error;
   mongoc_apm_command_failed_get_error (event, &error);
   SEED_WARNING <<"Command "<< mongoc_apm_command_failed_get_command_name (event)
   				<< " failed:" << error.message << "\n";
}

Database::Database(const char *databaseName) {

	dbName = databaseName;
	bson_t *command, reply;
	bson_error_t error;
	mongoc_apm_callbacks_t *callbacks;

	mongoc_init ();
	// Create a new client
	client = mongoc_client_new ("mongodb://127.0.0.1/");

	mongoc_client_set_error_api (client, 2);
   	callbacks = mongoc_apm_callbacks_new ();

	// mongoc_apm_set_command_started_cb (callbacks, command_started);	// really need more debug.
	mongoc_apm_set_command_succeeded_cb (callbacks, command_excuted);
	mongoc_apm_set_command_failed_cb (callbacks, command_failed);
	mongoc_client_set_apm_callbacks (client, callbacks, NULL);

	dtb = mongoc_client_get_database (client, dbName);
	command = BCON_NEW ("ping", BCON_INT32 (1));
	if (!mongoc_client_command_simple (client, "admin", command, NULL, &reply, &error)) {
		SEED_ERROR << error.message << "\n\n" << "[Suggestion] Lets try turn on mongo: $ sudo service mongod start";
	}

	bson_destroy (command);
	SEED_LOG << "Database \""<< databaseName << "\" has connected successful !";
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

	SEED_LOG << "Command insert to \"" << COLL_NAME << "\" collection";

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
		SEED_WARNING << error.message;
		return RET_FAILURE;

}

int Database::InsertDataWithIdAutoIncrement(const char *COLL_NAME, char* json) {

	Json *root = JsonParse(json);
    int total;
    bson_error_t error;
	const bson_t *doc;
	bson_t *query = BCON_NEW ("_id", "ob_id");

	db_collection *colt = mongoc_client_get_collection (client, dbName, COLL_NAME);

	// get value total
	mongoc_cursor_t *cursor = mongoc_collection_find (colt, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

	while (mongoc_cursor_more (cursor) && mongoc_cursor_next (cursor, &doc)) {
		char *str = bson_as_json (doc, NULL);
		Json *tot = JsonParse(str);
		Json *item = JsonGetArrayItem(tot, 1);	// get item "total"
		// add : "_id" : total+1
		total = JsonItemValInt(item)+1;
		JsonAddNumberToObject(root, "_id", total);
		cJSON_Delete(tot);
		bson_free (str);
	}

	if (mongoc_cursor_error (cursor, &error)) {
		cJSON_Delete(root);
		bson_destroy (query);
		mongoc_cursor_destroy (cursor);
		mongoc_collection_destroy (colt);
		SEED_WARNING << "An error occurred: " << error.message;
		return RET_FAILURE;
	}

	mongoc_cursor_destroy (cursor);
	// insert a new data
    if( InsertData(COLL_NAME, cJSON_Print(root)) == RET_FAILURE ) {
    	cJSON_Delete(root);
    	bson_destroy (query);
    	mongoc_collection_destroy (colt);
    	SEED_WARNING << "InsertData() error";
    	return RET_FAILURE;
    }
    
    cJSON_Delete(root);
    

	bson_t *update = BCON_NEW ("$set", "{", "total", BCON_INT32 (total), "}");
	
	// modifile the total number of documents in a collection
	if (!mongoc_collection_find_and_modify (colt, query, NULL, update, NULL, false, false, true, NULL, &error)) {
		bson_destroy (query);
		bson_destroy (update);
		mongoc_collection_destroy (colt);
		SEED_WARNING << error.message;
		return RET_FAILURE;
	}

	bson_destroy (query);
	bson_destroy (update);
	mongoc_collection_destroy (colt);
    return RET_SUCCESS;
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
		SEED_WARNING << error.message;
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
		SEED_WARNING << error.message;
		return RET_FAILURE;
}

// object.TotalDocuments("collection name");
int64_t Database::TotalDocuments (const char *COLL_NAME){

   	int64_t count;
   	bson_error_t error;
   	db_collection *colt = mongoc_client_get_collection (client, dbName, COLL_NAME);

	if ( (count = mongoc_collection_count (colt, MONGOC_QUERY_NONE, NULL, 0, 0, NULL, &error)) < 0) {
		mongoc_collection_destroy (colt);
		SEED_WARNING << error.message;
		return RET_FAILURE;
	} else {
		mongoc_collection_destroy (colt);
		SEED_LOG << "The \"" << COLL_NAME << "\" collection has: " << count << " documents" ;
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

