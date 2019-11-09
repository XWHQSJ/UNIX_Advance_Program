#include "apue.h"
#include "error.h"
#include <fcntl.h>

#define DEPTH 1000
#define STARTDIR "/tmp"
#define NAME "alonglonglonglonglonglonglonglonglonglongname"
#define MAXSZ (10*8192)

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
    int i;
    char *ptr;
    size_t size;
    
    if (chdir(STARTDIR) < 0)
    {
        err_sys("chdir failed");
    }

    for (i = 0; i < DEPTH; ++i)
    {
        if (mkdir(NAME, DIR_MODE) < 0)
        {
            err_sys("mkdir failed, i = %d", i);
        }

        if (chdir(NAME) < 0)
        {
            err_sys("chdir failed, i = %d", i);
        }
    }

    if (creat("afile", FILE_MODE) < 0)
    {
        err_sys("create error");
    }

    ptr = path_alloc(&size);

    for (; ;)
    {
        if (getcwd(ptr, size) != NULL)
        {
            break;
        }
        else
        {
            err_ret("getcwd failed, size = %ld", (long)size);
            size += 100;

            if (size > MAXSZ)
            {
                err_quit("giving up");
            }

            if ((ptr = realloc(ptr, size)) == NULL)
            {
                err_sys("realloc error");
            }
        }
    }

    printf("length = %ld\n%s\n", (long)strlen(ptr), ptr);

    exit(0);
}