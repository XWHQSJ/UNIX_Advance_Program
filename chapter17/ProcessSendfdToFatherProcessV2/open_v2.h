#include <apue.h>
#include <error.h>

// client's request for server
#define CL_OPEN "open"

// server's well-known name
#define CS_OPEN "/tmp/opend.socket"

int csopen(char*, int);