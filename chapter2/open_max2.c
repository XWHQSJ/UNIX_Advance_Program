#include "apue.h"
#include "error.h"
#include <errno.h>
#include <limits.h>
#include <sys/resource.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

// if OPEN_MAX is indeterminate, this might be inadeqate
#define OPEN_MAX_GUESS 256

long open_max(void)
{
    struct rlimit r1;

    // first time through
    if (openmax == 0)
    {
        errno = 0;
    }

    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 || openmax == LONG_MAX)
    {
        if ((openmax = getrlimit(RLIMIT_NOFILE, &r1)) < 0)
        {
            err_sys("can not get file limit\n");
        }
        else if (openmax == RLIM_INFINITY)
        {
            openmax = OPEN_MAX_GUESS;
        }
        else
        {
            openmax = r1.rlim_max;
        }
    }

    return (openmax);
}

int main(void)
{
    long i = 0;
    if (i = open_max())
    {
        printf("%ld\n", i);
        printf("successful!\n");
    }
    else
    {
        printf("failure!\n");
    }

    return 0;
}