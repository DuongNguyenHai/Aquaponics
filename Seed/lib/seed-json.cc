#include "seed-json.h"

bool JsonValid(char *string){

	json_object * jobj = json_tokener_parse(string);
	if(jobj == NULL){
		#if LOG
		sd_bug("### : invalid json !");
		sd_bug(string);
		#endif
		return 0;
	}
	free(jobj);
	return 1;
}

bool JsonParse(char *string){

	json_object * jobj = json_tokener_parse(string);
	if(jobj == NULL){
		sd_bug("### : json error !");
		return 0;
	}
	enum json_type type;

	json_object_object_foreach(jobj, key, val) {

		type = json_object_get_type(val);

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
}

char * JsonCreateObject(const char *OBJECT_PROP[][2], long int numRows, void *values[5]){

	json_object * jobj = json_object_new_object();

	for(int i = 0; i < numRows ; i++){
		json_object *js;

		if(strcmp(OBJECT_PROP[i][1],TP_STRING_STR) == 0){
			js = json_object_new_string( VOID_STRING(values[i]));
		}else if (strcmp(OBJECT_PROP[i][1],TP_INT_STR) == 0){
			js = json_object_new_int( VOID_INT(values[i]));
		}else if(strcmp(OBJECT_PROP[i][1],TP_BOOL_STR) == 0){
			js = json_object_new_boolean( VOID_BOOL(values[i]));
		}else if(strcmp(OBJECT_PROP[i][1],TP_DOUBLE_STR) == 0){
			js = json_object_new_double( VOID_DOUBLE(values[i]));
		}else {
			#if LOG
			sd_log("### ");
			sd_log(OBJECT_PROP[i][1],NO_NEW_LINE);
			sd_log(": not shape of any type!");
			#endif
			continue;
		}
		json_object_object_add(jobj, OBJECT_PROP[i][0], js );
	}

	#if LOG
	sd_log("json object created: ",NO_NEW_LINE);
	sd_log(json_object_to_json_string(jobj));
	#endif

	return (char *)json_object_to_json_string(jobj);
}

static char *ToLowerCase(char *s){
    int i = 0;
    char    *str = strdup(s);
    while (str[i]){
        if (str[i] >= 65 && str[i] <= 90)
            str[i] += 32;
        i++;
    }
    return (str);
}

static bool GetAnyValueJSON(struct sd_struct_json *json_data, char *s, jsmntok_t *tok){

	if(tok->type == JSMN_STRING){
		VOID_POINT_STRING(json_data->val, s );
		json_data->type = TP_STRING;

	}else if(tok->type == JSMN_PRIMITIVE){
		
		s = ToLowerCase(s);

		if( strcmp(s,"true") == 0 ){
			VOID_POINT_BOOL(json_data->val, true );
			json_data->type = TP_BOOL;
		}else if(strcmp(s,"false") == 0){
			VOID_POINT_BOOL(json_data->val, false );
			json_data->type = TP_BOOL;
		}else if(strcmp(s,"null") == 0){

		}else {

			for(int i=0; i< (tok->end - tok->start) ; i++){
				if(s[i]=='.'){
					VOID_POINT_DOUBLE(json_data->val, atof(s) );
					json_data->type = TP_DOUBLE;
					break;
				}
				if(i == ( tok->end - tok->start -1 ) ){
					VOID_POINT_INT(json_data->val, atoi(s) );
					json_data->type = TP_INT;
					break;
				}
			}
		}
	}
	return 1;
}

static int sd_get_json(struct sd_struct_json *json_data, char *json_string){
	
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

void Print_sd_struct_json(struct sd_struct_json *json_data){
	
	int j = 0;
	while(1){
		if(json_data[j].name == NULL){
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
}