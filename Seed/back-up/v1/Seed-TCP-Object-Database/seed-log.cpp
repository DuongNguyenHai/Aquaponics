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