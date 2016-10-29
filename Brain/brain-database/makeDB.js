// command line : mongo < file.sh

// db.runCommand({whatsmyuri : 1});

// connect to database
db = connect("localhost:34886");

// get user name
// db.getUsers();

// show all databases
// db.adminCommand('listDatabases');

// use database : "duong"
db = db.getSiblingDB('SongHe');

// show all collections
db.getCollectionNames();

