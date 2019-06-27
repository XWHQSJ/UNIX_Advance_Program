#include "apue.h"
#include "error.h"

static void charatatime(char *);

int main(void)
{
    pid_t pid;

    TELL_WAIT();

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid == 0)
    {
        // parent goes first
        WAIT_PARENT();

        charatatime("output from child\n");
    }
    else
    {
        charatatime("output from parent\n");

        TELL_CHILD(pid);
    }
    
    exit(0);
}

static void charatatime(char *str)
{
    char *ptr;
    int c;

    // set unbuffered
    setbuf(stdout, NULL);

    for(ptr = str; (c = *ptr++) != 0; )
    {
        putc(c, stdout);
    }
}