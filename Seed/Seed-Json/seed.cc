#include "../lib/seed-json.h"
#include <stdio.h>

char *string = (char *)"{\"sitename\" : \"joys of programming\","
	"\"tags\" : [ \"c\" , \"c++\", \"java\", \"PHP\" ],"
	"\"author-details\": { \"name\" : \"Joys of Programming\", \"Number of Posts\" : 10 } }";

char *stringError = (char *)"{\"sitename\" : \"joys of programming\","
					"\"tags\" : [ \"c\" . \"c++\", \"java\", \"PHP\" ],"
					"}";
char *JSON_STRING =
	(char *)"{\"command\":{\"user\": \"Duong\", \"admin\": true}, \"ID\":[\"uid\": 1000], \"point\":9.6,\"star\":3 }";

const char *SENSOR_PROP[][2] = 	{ 
									{"temp",TP_INT_STR},
									{"heartTemp",TP_DOUBLE_STR},
									{"state",TP_BOOL_STR},
									{"warning", TP_STRING_STR}
								};

void **values = (void **)malloc(sizeof(void *)*5);
char *str = (char *)"the temperature is so high ! ";

int main(int argc, char const *argv[]){

	// test valid json
	printf("+++ Test valid string json\n");
	// if(JsonValid(string))
		// printf("... Test pass\n");
	// else
		// printf("!!! Test false\n");
	
	// test un valid json
	printf("+++ Test invalid string json\n");
	// if(!JsonValid(stringError))
	// 	printf("... Test pass\n");
	// else
	// 	printf("!!! Test false\n");

	// test parse json-string
	// JsonParse(string);

	// // test create a new object json
	printf("+++ Test creat a new string json\n");

	// VOID_POINT_INT(values[0],60);
	// VOID_POINT_DOUBLE(values[1],30.12345);
	// VOID_POINT_BOOL(values[2],true);
	// VOID_POINT_STRING(values[3],str);
	// VOID_POINT_STRING(values[4],str);

	// char *json_created = JsonCreateObject(SENSOR_PROP, rows_of_array(SENSOR_PROP), values);
	// if(json_created != NULL)
	// 	if(JsonValid(json_created))
	// 		printf("... Test pass\n");
	// free(json_created);

	// test get data from string json
	printf("+++ get data from string json\n");
	struct sd_struct_json *sd_json_data = json_parse(string);
	Print_sd_struct_json(sd_json_data);
	return 0;
}