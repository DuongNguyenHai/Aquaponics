// Nguyen Hai Duong
// Date : july 2016

#include "seed-log.h"

// Show error message
void sd_error(const char *msg){

#ifdef DEBUG
    perror(msg);
    exit(1);
#endif

}

// show information
void sd_log(const char *msg, bool NEWLINE){

	fprintf (stdout, "%s", msg);
	if(NEWLINE)
		fprintf(stdout, "\n");

}

// show bug
void sd_bug(const char *msg, bool NEWLINE){

#ifdef DEBUG

	fprintf(stderr,"%s",msg);
	if(NEWLINE)
		fprintf(stdout, "\n");

#endif
}