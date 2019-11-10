#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <stropts.h>

int ptym_open(char* pts_name, int pts_namesz)
{
    char *ptr;
    int fdm, err;

    if((fdm = posix_openpt(O_RDWR)) < 0)
    {
        return -1;
    }

    if(grantpt(fdm) < 0)
    {
        goto errout;
    }

    if((ptr = ptsname(fdm)) == NULL)
    {
        goto errout;
    }

    strncpy(pts_name, ptr, pts_namesz);
    pts_name[pts_namesz - 1] = '\0';
    return(fdm);

errout:
    err = errno;
    close(fdm);
    errno = err;
    return -1;
}

int ptys_open(char *pts_name)
{
    int fds;

    if((fds = open(pts_name, O_RDWR)) < 0)
    {
        return -1;
    }

    return fds;
}