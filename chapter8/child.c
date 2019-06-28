#include "apue.h"
#include "error.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd, fd_flags;
    
    sscanf(argv[1], "%d", &fd);

    if ((fd_flags = fcntl(fd, F_GETFD)) < 0)
    {
        err_sys("get fd flags error");
    }

    if (fd_flags & FD_CLOEXEC)
    {
        printf("close on exec is on\n");
    }
    else
    {
        printf("close on exec is off\n");
    }
    exit(0);
}
