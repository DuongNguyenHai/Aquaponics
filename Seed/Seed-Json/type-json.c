#include "../lib-others/lib-json/include/json-c/json.h"
#include <stdio.h>
#include <string.h>

int main() {

	char *str = (char *)"{\"name\" : \"joys of programming\"}";

	json_object * jobj = json_tokener_parse(str);

	enum json_type type = json_object_get_type(jobj);

	printf("type: [%d] ",type);

	switch (type) {
		case json_type_null: printf("json_type_null\n");
			break;
		case json_type_boolean: printf("json_type_boolean\n");
			break;
		case json_type_double: printf("json_type_double\n");
			break;
		case json_type_int: printf("json_type_int\n");
			break;
		case json_type_object: printf("json_type_object\n");
			break;
		case json_type_array: printf("json_type_array\n");
			break;
		case json_type_string: printf("json_type_string\n");
			break;
	}
}
