// command line : mongo < file.sh

var tempt = "temptTank"
var DO = "DO"
var amoniac = "amoniac"
var nitrit = "nitrit"
var nitrat = "nitrat"
var hardness = "hardness"

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


function getNextSequenceValue(sequenceName, docName){

   var sequenceDocument = db[docName].findAndModify({
      query:{_id: sequenceName },
      update: {$inc:{total:1}},
      new:true
   });
	
   return sequenceDocument.total;
}

// Temperature of tank
db[tempt].insert({_id:"ob_id", total:0})
db[tempt].insert([
	{
	   "_id" : getNextSequenceValue("ob_id", tempt),
	   "tempt1" : "describe infor",
	   "tempt2" : "describe infor"
	}
])

// DO of tank
db[DO].insert({_id:"ob_id", total:0})
db[DO].insert([
	{
	   "_id" : getNextSequenceValue("ob_id", DO),
	   "DO1" : "describe infor",
	   "DO2" : "describe infor"
	}
])

// amoniac
db[amoniac].insert({_id:"ob_id", total:0})
db[amoniac].insert([
	{
	   "_id" : getNextSequenceValue("ob_id", amoniac),
	   "amoniac" : "describe infor"
	}
])


// nitrit
db[nitrit].insert({_id:"ob_id", total:0})
db[nitrit].insert([
	{
	   "_id" : getNextSequenceValue("ob_id", nitrit),
	   "nitrit" : "describe infor"
	}
])

// nitrat
db[nitrat].insert({_id:"ob_id", total:0})
db[nitrat].insert([
	{
	   "_id" : getNextSequenceValue("ob_id", nitrat),
	   "nitrat" : "describe infor"
	}
])

// hardness of tank
db[hardness].insert({_id:"ob_id", total:0})
db[hardness].insert([
	{
	   "_id" : getNextSequenceValue("ob_id", hardness),
	   "hardness" : "describe infor"
	}
])


// show all collections
db.getCollectionNames();