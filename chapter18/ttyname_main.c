#include "apue.h"
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct devdir {
    struct devdir *d_next;
    char *d_name;
};

static struct devdir *head;
static struct devdir *tail;
static char pathname[_POSIX_PATH_MAX + 1];
static char ctermid_name[L_ctermid];

char* ctermid(char *str)
{
    if(str == NULL)
    {
        str = ctermid_name;
    }

    return (strcpy(str, "/dev/tty"));
}

int isatty(int fd)
{
    struct termios  ts;
    return(tcgetattr(fd, &ts) != -1);
}

static void add(char *dirname)
{
    struct devdir *ddp;
    int len;

    len = strlen(dirname);

    if((dirname[len - 1] == '.') && (dirname[len - 2] == '/' ||
    (dirname[len - 2] == '.' && dirname[len - 3] == '/')))
        return;
    
    if(strcmp(dirname, "/dev/fd") == 0)
    {
        return;
    }

    if((ddp = malloc(sizeof(struct devdir))) == NULL)
    {
        return;
    }

    if((ddp->d_name = strdup(dirname)) == NULL)
    {
        free(ddp);
        return;
    }

    ddp->d_next = NULL;
    if(tail == NULL)
    {
        head = ddp;
        tail = ddp;
    }
    else
    {
        tail->d_next = ddp;
        tail = ddp;
    }
    
}

static void cleanup(void)
{
    struct devdir *ddp, *nddp;
    ddp = head;
    while (ddp != NULL)
    {
        nddp = ddp->d_next;
        free(ddp->d_name);
        free(ddp);
        ddp = nddp;
    }
    head = NULL;
    tail = NULL;
}

static char* searchdir(char* dirname, struct stat* fdstatp)
{
    struct stat devstat;
    DIR *dp;
    int devlen;
    struct dirent *dirp;

    strcpy(pathname, dirname);
    if((dp = opendir(dirname)) == NULL)
    {
        return(NULL);
    }

    strcat(pathname, "/");
    devlen = strlen(pathname);
    while ((dirp = readdir(dp)) != NULL)
    {
        strncpy(pathname + devlen, dirp->d_name, _POSIX_PATH_MAX - devlen);

        if(strcmp(pathname, "/dev/stdin") == 0 ||
        strcmp(pathname, "/dev/stdout") == 0 ||
        strcmp(pathname, "/dev/stderr") == 0)
        {
            continue;
        }

        if(stat(pathname, &devstat) < 0)
        {
            continue;
        }
        if(S_ISDIR(devstat.st_mode))
        {
            add(pathname);
            continue;
        }
        if(devstat.st_ino == fdstatp->st_ino &&
        devstat.st_dev == fdstatp->st_dev)
        {
            closedir(dp);
            return(pathname);
        }
    }

    closedir(dp);
    return NULL;
}

char * ttyname(int fd)
{
    struct stat fdstat;
    struct devdir *ddp;
    char *rval;

    if(isatty(fd) == 0)
    {
        return(NULL);
    }
    
    if(fstat(fd, &fdstat) < 0)
    {
        return(NULL);
    }

    if(S_ISDIR(fdstat.st_mode) == 0)
    {
        return(NULL);
    }

    rval = searchdir("/dev", &fdstat);
    if(rval == NULL)
    {
        for(ddp = head; ddp != NULL; ddp = ddp->d_next)
        {
            if((rval = searchdir(ddp->d_name, &fdstat)) != NULL)
            {
                break;
            }
        }
    }

    cleanup();
    return(rval);
}

int main(void)
{
    char *name;
    if(isatty(0))
    {
        name = ttyname(0);
        if(name == NULL)
        {
            name = "undefined";
        }
    }else
    {
        name = "not a tty";
    }
    printf("fd 0: %s\n", name);
    
    if(isatty(1))
    {
        name = isatty(1);
        if(name = NULL)
        {
            name = "undefined";
        }
    }else
    {
        name = "not a tty";
    }
    printf("fd 1: %s\n", name);

    if(isatty(2))
    {
        name = isatty(2);
        if(name = NULL)
        {
            name = "undefined";
        }
    }else
    {
        name = "not a tty";
    }
    printf("fd 2: %s\n", name);
    
    exit(0);
}