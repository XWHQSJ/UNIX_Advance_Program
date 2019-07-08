#include "apue.h"
#include "error.h"
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
    static long openmax = OPEN_MAX;
#else
    static long openmax = 0;
#endif

// if OPEN_MAX is indeterminate, this might be inadeqate
#define OPEN_MAX_GUESS 256

long open_max(void)
{
    // first time through
    if(openmax == 0)
    {
        errno = 0;
    }

    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0)
    {
        if (errno == 0)
        {
            // it's indeterminate
            openmax = OPEN_MAX_GUESS;
        }
        else
        {
            err_sys("sysconf error for _SC_OPNE_MAX");
        }
    }

    return (openmax);
}

int main(void)
{
    int i = 0;
    if(i = open_max())
    {
        printf("%d\n", i);
        printf("successful!\n");
    }
    else
    {
        printf("failure!\n");
    }
    

    return 0;
}