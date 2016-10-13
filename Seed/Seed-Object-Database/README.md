# Chương trình thực hiện các tác vụ cơ bản với database

- connect to database 
- Insert data
- Update data
- Delete data

# API - DATABASE:

+ Green	bool ConnectDatabase(const char *DATABASE_NAME);

	// Open a collection
	// mongoc_collection_t *coll = OpenCollection();
+	mongoc_collection_t *OpenCollection(const char *DATABASE_NAME, const char *coltName);

	// query
+	bool InsertData(mongoc_collection_t *colt,char *json);
+	bool UpdateData(mongoc_collection_t *colt,char *json, char *jsonQuery);
+	bool DeleteData(mongoc_collection_t *colt,char *jsonQuery);
+	int64_t TotalDocuments (mongoc_collection_t *colt);
	
	// Close collection
+	void DisconnectDatabase();

	// The alternative way of OpenCollection : OpenCollection2(&colt,DATABASE_NAME, sensor); // mongoc_collection_t *colt
+	bool OpenCollection2(mongoc_collection_t **colt,const char *DATABASE_NAME, const char *coltName);


