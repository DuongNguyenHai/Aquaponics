// #define __USE_C99_MATH

#include <stdio.h>
#include <stdbool.h>	// for bool type
#include <stdlib.h>     /* atoi */
#include <string.h>
#include "../lib-others/lib-json-jsmn/include/jsmn/jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

// [1] 	Convert void *values[] to appreciate type (char,int,bool,double) to use
// 		it was created to work with  [2]
// 		+	VOID_STRING(values[i]);
// 		+	...
#define VOID_STRING(ARG) (char *)(ARG)
#define VOID_INT(ARG) *( (int *)ARG)
#define VOID_BOOL(ARG) *( (bool *)ARG)
#define VOID_DOUBLE(ARG) *( (double *)ARG)

// [2] 	marco function for void *values[]
// 		the purpose is to save many diff types in one array : void* values[]
// 		usage :
// 		+	VOID_POINT_STRING(values[i], data);
// 		+	...
#define VOID_POINT_STRING(ARG,VAL) ({ ARG = VAL; })
#define VOID_POINT_INT(ARG,VAL) ({  ARG = malloc(sizeof(int)); (*( (int *)ARG)) = VAL; })
#define VOID_POINT_BOOL(ARG,VAL) ({  ARG = malloc(sizeof(bool)); (*( (bool *)ARG)) = VAL; })
#define VOID_POINT_DOUBLE(ARG,VAL) ({  ARG = malloc(sizeof(double)); (*( (double *)ARG)) = VAL; })

// const char *JSON_STRING =
// 	"{\"user\": \"Duong\", \"admin\": True, \"uid\": 123, \"point\": 10.5}";

char *JSON_STRING =
	(char *)"{\"command\":{\"user\": \"Duong\", \"admin\": true}, \"ID\":[\"uid\": 1000], \"point\":9.6,\"star\":3 }";

char *ToLowerCase(char *s){
    int i = 0;
    char    *str = strdup(s);
    while (str[i]){
        if (str[i] >= 65 && str[i] <= 90)
            str[i] += 32;
        i++;
    }
    return (str);
}

typedef enum {
		TP_BOOL = 1,
		TP_INT = 2,
		TP_DOUBLE = 3,
		TP_STRING = 4,
} sd_enum_type;

typedef struct sd_struct_json {
	sd_enum_type type;
	jsmntype_t hash_type;

	char *name;
	void *val;
	struct sd_struct_json *sub;

} sd_struct_json;

bool GetAnyValueJSON(struct sd_struct_json *json_data, char *s, jsmntok_t *tok){

	if(tok->type == JSMN_STRING){
		
		// printf("- value is string : ");
		// printf("%s\n",s);
		VOID_POINT_STRING(json_data->val, s );
		json_data->type = TP_STRING;

	}else if(tok->type == JSMN_PRIMITIVE){
		
		s = ToLowerCase(s);

		if( strcmp(s,"true") == 0 ){
			// printf("- value is boolean : ");
			// printf("%s\n", s);
			VOID_POINT_BOOL(json_data->val, true );
			json_data->type = TP_BOOL;
		}else if(strcmp(s,"false") == 0){
			// printf("- value is boolean : ");
			// printf("%s\n", s);
			VOID_POINT_BOOL(json_data->val, false );
			json_data->type = TP_BOOL;
		}else if(strcmp(s,"null") == 0){

			// printf("- value is null : ");
			// printf("%s\n", s);

		}else {

			for(int i=0; i< (tok->end - tok->start) ; i++){
				if(s[i]=='.'){
					// printf("- value is double : ");
					// printf("%lf\n", atof(s));
					VOID_POINT_DOUBLE(json_data->val, atof(s) );
					json_data->type = TP_DOUBLE;
					break;
				}
				if(i == ( tok->end - tok->start -1 ) ){
					// printf("- value is int : ");
					// printf("%d\n", atoi(s));
					VOID_POINT_INT(json_data->val, atoi(s) );
					json_data->type = TP_INT;
					break;
				}
			}
		}
	}
	return 1;
}

int sd_get_json(struct sd_struct_json *json_data, char *json_string){
	
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	int r = jsmn_parse(&p, json_string, strlen(json_string), t, sizeof(t)/sizeof(t[0]));
	// printf("token:%d, size of strings:%zu\n",r ,strlen(json_string));

	int j,k;
	for(j=1, k=0;j<r;){

		switch(t[j].type){
			case JSMN_OBJECT: {
				printf("---------------------------------------------------------------------------\n");
				printf("* object : %.*s\n", t[j].end-t[j].start, json_string + t[j].start);
					// add type for data_name
					json_data[k].hash_type = JSMN_OBJECT;

					char *sub_json = (char *)malloc(sizeof(char));
					json_data[k].sub = (struct sd_struct_json *)malloc((t[j].size +1)*sizeof(struct sd_struct_json));
					strncpy(sub_json,json_string + t[j].start, t[j].end-t[j].start );

					// parse sub-json and count token for j
					j += sd_get_json(json_data[k].sub,sub_json);
					k++;
				}
				break;
			case JSMN_ARRAY : {
				printf("---------------------------------------------------------------------------\n");
				printf("* Array : %.*s\n", t[j].end-t[j].start, json_string + t[j].start);
					// add type for data_name
					json_data[k].hash_type = JSMN_ARRAY;

					char *sub_json = (char *)malloc(sizeof(char));
					json_data[k].sub = (struct sd_struct_json *)malloc((t[j].size +1)*sizeof(struct sd_struct_json));
					strncpy(sub_json,json_string + t[j].start, t[j].end-t[j].start );

					// parse sub-json and count token for j
					j += sd_get_json(json_data[k].sub,sub_json);
					k++;
				}
				break;
			default : {
					// printf("- %.*s\n", t[j].end-t[j].start, json_string + t[j].start);

					if(json_data[k].name == NULL){
						json_data[k].name = (char *)malloc(sizeof(char));
						strncpy(json_data[k].name,json_string + t[j].start, t[j].end-t[j].start );
						printf("- name    [%d]: %s\n",k, json_data[k].name);
					}else if(json_data[k].hash_type == JSMN_ARRAY){
						printf("has array\n");
						k++;
					}else {
						// printf("what the hell\n");
						char *s = (char *)malloc(sizeof(char));
						strncpy(s,json_string + t[j].start, t[j].end-t[j].start);
						printf("- value   [%d]: %.*s\n",k, t[j].end-t[j].start, json_string + t[j].start);
						GetAnyValueJSON(&json_data[k],s,&t[j]);
						// printf("%d\n", json_data[k].type);
						k++;
					}
				j++;
			}
		}
	
	}
	printf("---------------------------------------------------------------------------\n");
	return j; // very important, must to return j
}

struct sd_struct_json *json_parse(char * json_string){
	sd_struct_json *json_data = (struct sd_struct_json *)malloc(20*sizeof(struct sd_struct_json));
	sd_get_json(json_data, json_string);

	return json_data;
}

bool Print_sd_struct_json(struct sd_struct_json *json_data){
	
	int j = 0;
	while(1){
		if(json_data[j].name == NULL){
			// printf("break : %d\n",j);
			break;
		}else{
			switch(json_data[j].hash_type){
				case JSMN_OBJECT: {
					printf("---------------------------------------------------------------------------\n");
						printf("* Groups :  %s\n", VOID_STRING(json_data[j]).name);
						Print_sd_struct_json(json_data[j].sub);
						j++;
					}
					break;
				case JSMN_ARRAY: {
					printf("---------------------------------------------------------------------------\n");
					printf("* Array : %s\n", VOID_STRING(json_data[j]).name);
						Print_sd_struct_json(json_data[j].sub);
						j++;
					}
					break;
				default : {
						printf("+ %s\n", VOID_STRING(json_data[j]).name);

						if(json_data[j].val != NULL){

							switch(json_data[j].type){
								case TP_BOOL: printf("- %s\n", VOID_BOOL(json_data[j].val) ? "true" : "false" );
									break;
								case TP_INT : printf("- %d\n", VOID_INT(json_data[j].val));
									break;
								case TP_DOUBLE : printf("- %lf\n", VOID_DOUBLE(json_data[j].val));
									break;
								case TP_STRING : printf("- %s\n", VOID_STRING(json_data[j].val));
									break;
								default:;
							}
							j++;
						}
					};
			}
		}
	}
	printf("---------------------------------------------------------------------------\n");
	return 1;
}

int main() {

	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	int r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	// /* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	// printf("token:%d, size of strings:%zu\n",r ,strlen(JSON_STRING));
	
	// struct sd_struct_json *json_data2 = (struct sd_struct_json *)malloc(20*sizeof(struct sd_struct_json));
	
	printf("Get data from string json .\n");
	struct sd_struct_json *json_data = json_parse(JSON_STRING);

	// sd_get_json(json_data2,JSON_STRING);

	// printf("%s\n", VOID_STRING(json_data[0]).name);
	// printf("%s\n", VOID_STRING(json_data[0].val));
	// printf("%d\n", (json_data[0].hash_type));
	// printf("%s\n", VOID_STRING(json_data[2].name));
	// printf("%s\n", VOID_BOOL(json_data[1].val) ? "true" : "false" );
	// printf("%lf\n", VOID_DOUBLE(json_data[2].val));
	// printf("%lf\n", VOID_DOUBLE(json_data[3].val));
	printf("Print struct data of json .\n");
	Print_sd_struct_json(json_data);

	return 0;
}
