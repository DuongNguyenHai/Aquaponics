mongoc_client_t      	*client;
mongoc_database_t    	*database;
mongoc_collection_t  	*collection;
bson_t              	*command,
                    	reply,
                   		*insert;
bson_error_t          	error;
bson_oid_t 				oid;
char                 	*str;
bool                 	retval;


/*
* Do work. This example pings the database, prints the result as JSON and
* performs an insert
*/
command = BCON_NEW ("ping", BCON_INT32 (1));

retval = mongoc_client_command_simple (client, "admin", command, NULL, &reply, &error);

if (!retval) {
  fprintf (stderr, "%s\n", error.message);
  return EXIT_FAILURE;
}

str = bson_as_json (&reply, NULL);
	printf ("%s\n", str);

// Insert 

insert = bson_new ();
bson_oid_init (&oid, NULL);
BSON_APPEND_OID (insert, "_id", &oid);
BSON_APPEND_UTF8 (insert, "title", "temperature");
BSON_APPEND_UTF8 (insert, "data", "25");
BSON_APPEND_UTF8 (insert, "heartTemp", "35");
BSON_APPEND_UTF8 (insert, "status", "1");
BSON_APPEND_UTF8 (insert, "warning", "none");

// other way to write insert
// insert = BCON_NEW (		"temp", BCON_DOUBLE (25),
// 		                  	"heartTemp", BCON_INT32 (35),
// 		                  	"status", BCON_INT32 (1),
// 		                  	"warning", BCON_UTF8 ("none")
// 		               );


if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, insert, NULL, &error)) {
  fprintf (stderr, "%s\n", error.message);
}