#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void catch_function(int signo) {
    signal(SIGINT, catch_function);
    puts("Interactive attention signal caught.");
}

void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
    printf("Caught segfault at address %p\n", si->si_addr);
    exit(0);
}

int main(int argc, char const *argv[])
{

    if (signal(SIGINT, catch_function) == SIG_ERR) {
        fputs("An error occurred while setting a signal handler.\n", stderr);
        return EXIT_FAILURE;
    }

    // if (signal(SIGSEGV, catch_function) == SIG_ERR) {
    //     fputs("An error occurred while setting a signal handler.\n", stderr);
    //     return EXIT_FAILURE;
    // }
    
    int *foo = NULL;
    struct sigaction sa;

    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segfault_sigaction;
    sa.sa_flags   = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);

    /* Cause a seg fault */
    *foo = 1;

    while(1) {
        printf("Hello\n");
        sleep(1);
    }


    return 0;


}

/* 

Note : Each item level has to create a new link

    cJSON *obj = cJSON_GetObjectItem(root, "item"); // create a new link to item


*/