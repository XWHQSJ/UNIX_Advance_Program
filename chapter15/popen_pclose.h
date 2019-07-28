#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
* Pointer to array allocated at run-time.
 */
static pid_t *childpid = NULL;

/*
* From our open_max(), {Figure 2.17}
 */
static int maxfd;

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

FILE *popen(const char *cmdstring, const char *type)
{
    int i;
    int pfd[2];
    pid_t pid;
    FILE *fp;

    /* only allow "r" or "w" */
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0)
    {
        errno = EINVAL;
        return (NULL);
    }

    if (childpid == NULL) /* first time through */
    {
        /* allocate zeroed out array for child pids */
        maxfd = open_max();
        if ((childpid = (pid_t *)calloc(maxfd, sizeof(pid_t))) == NULL)
        {
            return (NULL);
        }
    }

    if (pipe(pfd) < 0)
    {
        return (NULL); /* errno set by pipe() */
    }

    if (pfd[0] >= maxfd || pfd[1] >= maxfd)
    {
        close(pfd[0]);
        close(pfd[1]);
        errno = EMFILE;
        return (NULL);
    }

    if ((pid = fork()) < 0)
    {
        return (NULL);
    }
    else if (pid == 0)
    {
        if (*type == 'r')
        {
            close(pfd[0]);
            if (pfd[1] != STDOUT_FILENO)
            {
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
        }
        else
        {
            close(pfd[1]);
            if (pfd[0] != STDIN_FILENO)
            {
                dup2(pfd[0], STDOUT_FILENO);
                close(pfd[0]);
            }
        }

        /* close all descriptors in childpid[] */
        for (i = 0; i < maxfd; ++i)
        {
            if (childpid[i] > 0)
            {
                close(i);
            }
        }

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    }

    /* parent continues... */
    if (*type == 'r')
    {
        close(pfd[1]);
        if ((fp = fdopen(pfd[0], type)) == NULL)
        {
            return (NULL);
        }
    }
    else
    {
        close(pfd[0]);
        if ((fp = fdopen(pfd[1], type)) == NULL)
        {
            return (NULL);
        }
    }
    childpid[fileno(fp)] = pid; /* remember child pid for this fd */
    return (fp);
}

int pclose(FILE* fp)
{
    int fd, stat;
    pid_t pid;

    if(childpid == NULL)
    {
        errno = EINVAL;
        return(-1);             /* popen() has never been called */
    }

    fd = fileno(fp);
    if(fd >= maxfd)
    {
        errno = EINVAL;
        return(-1);             /* invalid file descriptor */
    }

    if((pid = childpid[fd]) == 0)
    {
        errno = EINVAL;
        return(-1);             /* fp wasn't opened by popen() */
    }

    childpid[fd] = 0;
    if(fclose(fp) == EOF)
    {
        return(-1);
    }

    while(waitpid(pid, &stat, 0) < 0)
    {
        if(errno != EINTR)
        {
            return(-1);         /* error other than EINTR from waitpid() */
        }
    }

    return(stat);               /* return child's termination status */
}