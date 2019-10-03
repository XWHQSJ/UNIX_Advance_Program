#include "apue.h"
#include <errno.h>

#define CL_OPEN "open"

extern char errmsg[];       // error massage string to return to client
extern int oflag;           // open() flag: O_xxx ...
extern char *pathname;      // of file to open() for client

int cli_args(int, char**);
void handle_request(char *, int, int);