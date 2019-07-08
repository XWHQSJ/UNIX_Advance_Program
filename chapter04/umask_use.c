#include "apue.h"
#include "error.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(void)
{
    umask(0);
    if (creat("foo", RWRWRW) < 0)
    {
        err_sys("create error for foo\n");
    }

    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWGRP);
    if(creat("bar", RWRWRW) < 0)
    {
        err_sys("create error for bar\n");
    }

    exit(0);
}