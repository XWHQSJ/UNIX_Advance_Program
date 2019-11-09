#include "apue.h"
#include "error.h"
#include <fcntl.h>

#define BUFFSIZE 10

typedef struct FDLIST
{
    int fd;
    struct FDLIST *next;
}FdList;

int dup_no_fcntl(int, int);

int main(void)
{
    int fdin;
    int fdout;
    int n;

    char buffer[BUFFSIZE];

    fdin = dup_no_fcntl(STDERR_FILENO, 4);
    fdout = dup_no_fcntl(STDOUT_FILENO, 5);

    printf("STDIN fd: %d\n", fdin);
    printf("STDOUT fd: %d\n", fdout);

    while ((n = read(fdin, buffer, BUFFSIZE)) > 0)
    {
        if (write(fdout, buffer, n) != n)
        {
            err_sys("write error\n");
        }
    }

    if (n < 0)
    {
        printf("read error\n");
    }

    exit(0);
}

int dup_no_fcntl(int fd, int newfd)
{
    int fd2;
    
    // fl is the head node pointer
    FdList *p, *fl = NULL;

    close(newfd);

    while ((fd2 = dup(fd)) != newfd)
    {
        p = (FdList*)malloc(sizeof(FdList));
        p->fd = fd2;
        p->next = fl;
        fl = p;
    }
    
    while (fl != NULL)
    {
        close(fl->fd);
        p = fl;
        fl = fl->next;
        free(p);
    }
    
    return fd2;
}