#include <stdio.h>
#include <iostream>
#include "seed-config.h"
#include "seed-log.h"
#include "seed-parse-config.h"
#include "seed-branch.h"

using namespace TREE;

#define LOGFILE "seed-main.log" // the file is used for write log
static int DEBUG_LEVEL = 2; // DEBUG_DATABASE_LV was defined in seed-config.cc

int main(int argc, char const *argv[])
{
    ParseOptions po("../config/sys-config.cfg");

    Branch SongHe;
    SongHe.Start();
    while(1);

	return 0;
}

/* 

Note : Each item level has to create a new link

    cJSON *obj = cJSON_GetObjectItem(root, "item"); // create a new link to item


*/