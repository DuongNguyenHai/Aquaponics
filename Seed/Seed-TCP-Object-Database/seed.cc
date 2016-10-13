// Nguyen Hai Duong
// Date : july 2016

#include <stdio.h>      // for printf() and fprintf()
#include <string.h>
#include "../lib/TCP-IP.h"
#include "../lib/seed-database.h"
#include "../lib/seed-object.h"

#define DATABASE_NAME "duong"

int main(int argc, char const *argv[]){

	int servSock, clntSock;
    int n;
    char buffer[BUFFSIZE];
    char *json;

    ConnectDatabase(DATABASE_NAME);
	Leaf sensor(DATABASE_NAME,"sensor");

    servSock = CreateTCPServerSocket(PORT);
    clntSock = AcceptTCPConnection(servSock);

    while(1){
        n = recv(clntSock,buffer,BUFFSIZE,0);
        if ( n<0 ) 
            sd_error("ERROR reading from socket");
        else if( n>0 ){
            json = buffer;
            printf("json[%d]: %s\n",n,json);
            fflush(stdout);
            sensor.InsertLeafToDatabase(json);
            bzero(buffer,BUFFSIZE);
        }
        else{
            printf("Client disconnect !\n");
            break;
        }
    }

	return 0;
}
