#include "seed-json.h" // Print information and write to a log-file
#include <stdio.h>
using namespace TREE;

/* Parse text to JSON, then render back to text, and print! */
char * string = (char*)"{\"name\" : \"joys of programming\",\"what\":123}";
char text1[] =
    "{\n"
    "\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n"
    "\"format\": {\"type\":       \"rect\", \n"
    "\"width\":      1920, \n"
    "\"height\":     1080, \n"
    "\"interlace\":  false,\"frame rate\": 24\n"
    "}\n"
    "}";

char *str2 = (char *)"{\"color\":[\"blue\", \"green\"]}";

char *sch = (char *)"{\"schedule\":[\"2h30\", \"3h30\"]}";

char *temptTank = (char *)   "{\"data\":\n"
                                "{\n"
                                    "\"temptTank\":{\n"
                                        "\"tempt1\":25,\n"
                                        "\"tempt2\":26\n"
                                    "}\n"
                                "}\n"
                            "}";

char *set = (char*)"{\"set\":"
                                "{"
                                "\"temptTank\":{\"tempt1\": 1,\"tempt2\":0},"
                                "\"DO\":{\"DO1\":1, \"DO2\":0},"
                                "\"amoniac\":{\"amoniac1\":1},"
                                "\"nitrit\":{\"nitrit1\":1},"
                                "\"nitrat\":{\"nitrat1\":1},"
                                "\"hardness\":{\"hardness1\":1}"
                                "}"
                            "}";

std::string command[] = {"schedule","timer"};

int main(int argc, char const *argv[])
{
	
	/* Process each json textblock by parsing, then rebuilding: */
    // JsonIsValid(string);
    // JsonIsValid(text1);
    
    // Get object
    SEED_LOG << "Scan object JSON. We know name of the object. and we want prase its content";
    JsonIsValid(temptTank);
    Json *root = JsonParse(temptTank);
    Json *header = JsonGetObjectItem(root, "data");
    SEED_LOG << "object: " << JsonItemName(header);
    Json *item1 = JsonGetObjectItem(header, "temptTank");
    SEED_LOG << "object: "<< JsonItemName(item1);

    Json *tempt1 = JsonGetObjectItem(item1,"tempt1");
    SEED_LOG<< "\t item: " << JsonItemName(tempt1) << ", content: " << JsonItemValInt(tempt1);
    Json *tempt2 = JsonGetObjectItem(item1,"tempt2");
    SEED_LOG<< "\t item: " << JsonItemName(tempt2) << ", content: " << JsonItemValInt(tempt2);

    JsonDelete(root);

    SEED_LOG << "Scan array JSON. We know name of the array. and we want prase its content";
    JsonIsValid(str2);
    Json *root2 = JsonParse(str2);
    Json *header2 = JsonGetObjectItem(root2, "color");
    for (int i = 0; i < JsonArraySize(header2); ++i)
    {
        Json *item2 = JsonGetArrayItem(header2, i);
        SEED_LOG<< "\titem: " << JsonItemValStr(item2);
    }

    JsonDelete(root2);

    SEED_LOG << "Scan all json. We know struct of json but not know whats name of object or array";

    if(JsonIsValid(set)==RET_FAILURE)
        return 0;

    Json *root3 = JsonParse(set);
    Json *header3 = JsonGetArrayItem(root3, 0);
    SEED_LOG << "object: " << JsonItemName(header3);
    for (int i = 0; i < JsonArraySize(header3); ++i) {
        Json *item = JsonGetArrayItem(header3, i);
        SEED_LOG<< "\titem: " << JsonItemName(item);
        for (int i = 0; i < JsonArraySize(item); ++i)
        {
            Json *subitem = JsonGetArrayItem(item, i);
            SEED_LOG<< "\t\tsubitem: " << JsonItemName(subitem) << ", content: " << JsonItemValInt(subitem);
        }
    }  

    JsonDelete(root3);

    SEED_LOG << "Use JsonGetHeader() to find what command correspond to header ";
    if(JsonIsValid(sch)==RET_FAILURE)
        return 0;

    Json *root4 = JsonParse(sch);
    int index = JsonGetHeader(sch, command, sizeof(command)/sizeof(command[0]));
    Json *header4 = JsonGetObjectItem(root4, command[index].c_str()); // get item form data  
    SEED_LOG<< "command: " << JsonItemName(header4);
    switch(index) {
        case 0 : {
            
            for (int i = 0; i < JsonArraySize(header4); ++i)
            {
                Json *item = JsonGetArrayItem(header4, i);
                SEED_LOG<< "\titem: " << JsonItemValStr(item);
            }
        } break;
        case 1: {
        } break;
    }

    JsonDelete(root4);

	return 0;
}