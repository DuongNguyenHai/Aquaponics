// Nguyen Hai Duong
// Date : july 2016

#ifndef SEED_LOG_H
#define SEED_LOG_H

#include <stdio.h>	// for printf() and fprintf()
#include <stdlib.h> // for perror

#define DEBUG true

#define NO_NEW_LINE false

// show information of processing
void sd_log(const char *msg, bool NEWLINE = true);

// show all bug
void sd_bug(const char *msg, bool NEWLINE = true);

// Show error message
void sd_error(const char *msg);


#endif