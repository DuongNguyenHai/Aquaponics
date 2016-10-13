#include <stdio.h>
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for bzero(), memset() */


#define BUFFSIZE 37		// contain "seed>"
#define DATASIZE 32

int main(int argc, char const *argv[])
{
	char buffer[BUFFSIZE];
	char data[DATASIZE];

	printf("enter your command\n");
	for(;;){
		bzero(buffer,BUFFSIZE);
		printf("seed> ");
		fgets(buffer,BUFFSIZE,stdin);
	}
	return 0;
}