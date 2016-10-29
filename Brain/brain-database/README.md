### Three query command for database.

-	Insert data
-	Update data
-	Delete data

### Function - DATABASE:

```objc

*	Initial to database

	Database(const char *DATABASE_NAME); // dt is name of object.

*	Query database

	bool InsertData(const char *COLL_NAME,char *json);
	bool UpdateData(const char *COLL_NAME, char *json, char *jsonSelector);
	bool DeleteData(const char *COLL_NAME, char *jsonSelector);
	
*	Display the total of number of documents in one colletion

	int64_t TotalDocuments (const char *COLL_NAME);

*	Display the total of number of query commands (all over the objects)

	void DisplayTotalQuery();

### Usage

Database was write as class so just initialing an object : Database dt("database name") 
and use it normally.


```

