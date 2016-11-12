
#include <stdio.h>
#include <iostream>
#include "seed-json.h" // Print information and write to a log-file
#include "seed-database.h"
#include "TCP-IP.h"
#include "seed-branch.h"

using namespace TREE;

#define DATABASE_NAME "SongHe"
#define SENSOR "sensor"

// std::string header[] = {"device","data"};

int main(int argc, char const *argv[])
{
    // Database dt(DATABASE_NAME);

	// char * dev = (char*)"{\"device\":"
 //                                "{"
 //                                "\"tempt\":{\"tempt1\": 1,\"tempt2\":1}, \n"
 //                                "\"DO\":{\"DO1\":1}, \n"
 //                                "\"amoniac\":{\"amoniac1\":1}, \n"
 //                                "\"nitrit\":{\"nitrit1\":1}, \n"
 //                                "\"nitrat\":{\"nitrat1\":1}, \n"
 //                                "\"hardness\":{\"hardness1\":1} \n"
 //                                "}"
 //                            "}";

    // char *data = (char *)   "{\"data\":"
    //                             "{"
    //                                 "\"tempt\":{"
    //                                     "\"tempt1\":25,\n"
    //                                     "\"tempt2\":26\n"
    //                                 "}"
    //                             "}"
    //                         "}";

    // char *arr = (char *) "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
	/* Process each json textblock by parsing, then rebuilding: */
    // JsonIsValid(dev);
    // JsonIsValid(data);

    Branch SongHe;
    SongHe.Start();
    // Twig Song;
    // int sizehd = sizeof(header)/sizeof(header[0]);
    // printf("number of command : %d\n", sizehd);

    // int index = JsonGetHeader(dev, header , sizehd);
    // if(index>=0)
    //     printf("The command is : %s\n", header[index].c_str());
    // else
    //     printf("Not found any command\n");

    // index = JsonGetHeader(data, header , sizehd);
    // if(index>=0)
    //     printf("The command is : %s\n", header[index].c_str());
    // else
    //     printf("Not found any command\n");

    // cJSON *root = cJSON_Parse(dev);
    // cJSON * format = cJSON_GetObjectItem(root,"device");
    // cJSON *tempt = cJSON_GetObjectItem(format,"tempt");



    // printf("format : %s\n", format->string);
    // if(cJSON_HasObjectItem(cJSON_GetObjectItem(root,"device"), "nitrat"))
    //     printf("oh\n");
    // else
    //     printf("oops\n");
   
    
    

    while(1);
    // Json *root = JsonParse(dev);
    // Json *device = JsonGetItem(root, "device");
    // printf("size of arrary : %d\n", JsonArraySize(device));
    // for (int i = 0 ; i < JsonArraySize(device) ; i++) {

    //     cJSON * subitem = JsonGetArrayItem(device, i);
    //     printf("%s\n", JsonItemVal(subitem));  
    // }
	return 0;
}

/* 

Note : Each item level has to create a new link

    cJSON *obj = cJSON_GetObjectItem(root, "item"); // create a new link to item


*/