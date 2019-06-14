#include "apue.h"
#include "error.h"

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;

// if PATH_MAX is indeterminate, no guarantee this is adequate
#define PATH_MAX_GUESS 1024

char *path_alloc(size_t *sizep)
{
    char *ptr;
    size_t size;

    if (posix_version == 0)
    {
        posix_version = sysconf(_SC_VERSION);
    }

    if (xsi_version == 0)
    {
        xsi_version = sysconf(_SC_XOPEN_VERSION);
    }

    // first time through
    if (pathmax == 0)
    {
        errno = 0;

        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
        {
            if (errno == 0)
            {
                // it is indeterminate
                pathmax = PATH_MAX_GUESS;
            }
            else
            {
                err_sys("pathconf error for _PC_PATH_MAX");
            }
        }
        else
        {
            pathmax++;
        }
    }

    // before POSIX.1-2001, we aren't guaranteed that PATH_MAX includes
    // the terminating null byte. Same goes for XPG3
    if ((posix_version < 200112L) && (xsi_version))
    {
        size = pathmax + 1;
    }
    else
    {
        size = pathmax;
    }

    if ((ptr = malloc(size)) == NULL)
    {
        err_sys("malloc error for pathname");
    }

    if (sizep != NULL)
    {
        *sizep = size;
    }

    return (ptr);
}

int main(void)
{
    char *ptr;
    size_t size;
    
    if (chdir("/usr/spool/uucppublic") < 0)
    {
        err_sys("chdir failed");
    }

    ptr = path_alloc(&size);

    if (getcwd(ptr, size) == NULL)
    {
        err_sys("getcwd failed");
    }

    printf("cwd = %s\n",ptr);

    exit(0);
}