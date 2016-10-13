// Nguyen Hai Duong
// Date : 11 jun 2016

#include "TCP-IP.h"
#include "TCP-IP.c"
#include <sys/wait.h>/* for waitpid() */

int main(int argc, char const *argv[])
{
	int servSock, clntSock;
    pid_t processID;
    unsigned int childProcCount = 0;

    servSock = CreateTCPServerSocket(PORT);

    while(1){

        clntSock = AcceptTCPConnection(servSock);
        /* Fork child process and report any errors */
        if ( (processID = fork()) < 0)
            error("fork() failed");
        else if (processID == 0) /* This is the child process */
        {
            close(servSock);
            /* Child closes listening socket */
            printf("Child process %d\n", (int) processID);
            HandleForkClient(clntSock);
            printf("Process terminates\n");
            exit(0);

            /* Child process terminates */
        }/* Child closes listening socket*/
        
        printf("processID : %d\n", (int) processID);
        close(clntSock);
        /* Parent closes child socket descriptor */
        childProcCount++;
        /* Increment number of outstanding child processes */
        while (childProcCount) /* Clean up all zombies */
        {
            printf("Before waitpid. Child : %d \nClear processID : %d\n", childProcCount, (int) processID);
            processID = waitpid((pid_t) -1, NULL, WNOHANG); /* Nonblocking wait */
            if (processID < 0) /* waitpid() error? */
                error("waitpid() failed");
            else if (processID == 0) /* No zombie to wait on */
                {
                    printf("else if processID : %d\n", (int) processID);
                    break;
                }
                else{
                    childProcCount--; /* Cleaned up after a child */
                    printf("After waitpid. Child :  %d\n", childProcCount);
                }
        }
    }

	return 0;
}
