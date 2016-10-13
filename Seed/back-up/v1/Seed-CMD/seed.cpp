// Nguyen Hai Duong
// Date : july 2016

#include <stdio.h>      // for printf() and fprintf()
#include "../lib/TCP-IP.h"
#include "../lib/cmd-config.h"


int main(int argc, char const *argv[]){
	
    while(1){

        CreatCmdConfig();

        while(1){
            printf("main alert !\n");
            sleep(1);
        }
    }

	return 0;
}
