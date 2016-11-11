#include "seed-json.h" // Print information and write to a log-file
#include <stdio.h>
using namespace TREE;

/* Parse text to JSON, then render back to text, and print! */

int main(int argc, char const *argv[])
{
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
	/* Process each json textblock by parsing, then rebuilding: */
    JsonIsValid(string);
    JsonIsValid(text1);
	return 0;
}