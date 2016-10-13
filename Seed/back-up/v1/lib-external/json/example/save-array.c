// #define __USE_C99_MATH

#include <stdio.h>
#include <stdbool.h>	// for bool type
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n, \"duong\":123,  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\", \"hihi\"], \"name\": \"haha\" "
	"\"block\": [\"users\", \"wheel\", \"audio\", \"video\", \"hoho\"]}";

// const char *JSON_STRING =
// 	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  ""}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

struct list{
	bool block;
	char* str;
	struct list **jsonGroup;
};

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	// char *strData[30];
	// for(int i=0;i<30;i++){
	// 	strData[i] = (char*)malloc(3);
	// }
	struct list **jsonData = (struct list **)malloc(20 * sizeof(struct list *));
	for(int j=0;j<10;j++){
		jsonData[j] = (struct list *)malloc(sizeof(struct list));
		jsonData[j]->str = (char *)malloc(1);
	}

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	printf("token:%d, size of strings:%zu\n",r,strlen(JSON_STRING));
	

	for(int j=1, k=0;j<(r-2);){
		// printf("%d\n", j);
		if((t[j+1].type == JSMN_ARRAY)){

			printf("Groups:%d\n",t[j+1].size);
			jsonData[k]->jsonGroup = (struct list **)malloc(t[j+1].size * sizeof(struct list *));
			// jsonData[k]->jsonGroup
			// jsonData[k]->str=(char*)realloc(jsonData[k]->str,t[j+1].end-t[j+1].start);
			// jsonData[k]->strBlock = (char**)realloc(jsonData[k]->str,t[j+1].end-t[j+1].start);
			// strncpy(jsonData[k]->str,JSON_STRING + t[j+1].start, t[j+1].end-t[j+1].start);
			// jsonData[k]->block = true;
			// printf("[%d] * group : %s\n", k,(jsonData[k])->str);
			j=j+t[j+1].size +2;
		}else{
			jsonData[k]->str=(char*)realloc(jsonData[k]->str,t[j+1].end-t[j+1].start);
			strncpy(jsonData[k]->str,JSON_STRING + t[j+1].start, t[j+1].end-t[j+1].start );
			printf("[%d] - item : %s\n", k,(jsonData[k])->str);
			j=j+2;
		}
		// printf("%d\n", jsonData[k]->block);
		k++;
		
	}

	return 0;

	
	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
			/* We may use strndup() to fetch string value */
			// printf("%d\n", t[i+1].end-t[i+1].start);
			printf("- User: %.*s\n", t[i+1].end-t[i+1].start,JSON_STRING + t[i+1].start);
		
			// strData[i]=(char*)realloc(strData[i],t[i+1].end-t[i+1].start);
			// strncpy(strData[i],JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start );
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			// strData[i]=(char*)realloc(strData[i],t[i+1].end-t[i+1].start);
			// strncpy(strData[i],JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start );
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			// strData[i]=(char*)realloc(strData[i],t[i+1].end-t[i+1].start);
			// strncpy(strData[i],JSON_STRING + t[i+1].start, t[i+1].end-t[i+1].start );
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
			printf("- Groups:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			// printf("%d\n", t[i+1].size);
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}
	return 0;
}
