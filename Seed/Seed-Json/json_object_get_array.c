#include "../lib-others/lib-json/include/json-c/json.h"
#include <stdio.h>

void json_parse(json_object ** jobj) {
  enum json_type type;
  json_object_object_foreach(*jobj, key, val) {
    type = json_object_get_type(val);
    switch (type) {
      case json_type_array: printf("type: json_type_array, ");
                          *jobj = json_object_object_get(*jobj, key);
                          break;
    }
  }
}
int main() {
  char *string = "{ \"tags\": [ \"c++\", \"php\", \"java\"] }";
  json_object *jvalue;

  printf ("JSON string: %s\n", string);

  json_object *jobj = json_tokener_parse(string);

  json_parse(&jobj);

  int arraylen = json_object_array_length(jobj);
  printf("\nArray Length: %d\n",arraylen);
  for (int i=0; i< arraylen; i++){
    jvalue = json_object_array_get_idx(jobj, i);
    printf("value[%d]: %s\n",i, json_object_get_string(jvalue));
  }

}