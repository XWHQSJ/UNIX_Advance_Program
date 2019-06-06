#include "apue.h"
#include "error.h"

int main(void)
{
    struct stat statbuf;

    // turn on set-group-ID and turn off group-execute
    
    if (stat("foo", &statbuf) < 0)
    {
        err_sys("stat error for foo\n");
    }

    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_IXGRP) < 0)
    {
        err_sys("chmod error for foo\n");
    }

    // set absolute mode to "rw-r--r--"

    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))
    {
        err_sys("chmod error for bar");
    }

    exit(0);
}