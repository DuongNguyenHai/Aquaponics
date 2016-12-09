// command line : mongo < file.sh

var tempt = "temptTank"
var DO = "DO"
var amoniac = "amoniac"
var nitrit = "nitrit"
var nitrat = "nitrat"
var hardness = "hardness"

// db.runCommand({whatsmyuri : 1});

// connect to database
db = connect("localhost:27017");

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
function getTime(offset) {
	return new Date( new Date().getTime() + offset * 3600 * 1000);
}

var localDate = getTime(7);	// time zone is 7
// var localDate = getTime(7);	// time zone is 7
// Temperature of tank
db[tempt].insert({_id:"ob_id", total:0, tempt:"The temperature of fish tank", date: localDate})
// for (var i = 0; i < 100000; i++) {
// 	db[tempt].insert([
// 	{
// 	   "_id" : getNextSequenceValue("ob_id", tempt),
// 	   "tempt" : 20,
// 	   "date" : localDate
// 	}
// 	])
// }
// db[tempt].insert([
// 	{
// 	   "_id" : getNextSequenceValue("ob_id", tempt),
// 	   "tempt" : "The temperature of fish tank",
// 	}
// ])

// DO of tank
// db[DO].insert({_id:"ob_id", total:0, DO:"The DO of fish tank", date: localDate})
// db[DO].insert([
// 	{
// 	   "_id" : getNextSequenceValue("ob_id", DO),
// 	   "DO1" : "describe infor",
// 	}
// ])

// amoniac
// db[amoniac].insert({_id:"ob_id", total:0, amoniac:"The amoniac of fish tank", date: localDate})
// db[amoniac].insert([
// 	{
// 	   "_id" : getNextSequenceValue("ob_id", amoniac),
// 	   "amoniac" : "describe infor"
// 	}
// ])


// nitrit
// db[nitrit].insert({_id:"ob_id", total:0, nitrit:"The nitrit of fish tank", date: localDate})
// db[nitrit].insert([
// 	{
// 	   "_id" : getNextSequenceValue("ob_id", nitrit),
// 	   "nitrit" : "describe infor"
// 	}
// ])

// nitrat
// db[nitrat].insert({_id:"ob_id", total:0, nitrat:"The nitrat of fish tank", date: localDate})
// db[nitrat].insert([
// 	{
// 	   "_id" : getNextSequenceValue("ob_id", nitrat),
// 	   "nitrat" : "describe infor"
// 	}
// ])

// hardness of tank
// db[hardness].insert({_id:"ob_id", total:0, hardness:"The hardness of fish tank", date: localDate})
// db[hardness].insert([
// 	{
// 	   "_id" : getNextSequenceValue("ob_id", hardness),
// 	   "hardness" : "describe infor"
// 	}
// ])


// show all collections
db.getCollectionNames();