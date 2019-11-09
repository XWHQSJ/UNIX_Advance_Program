#include "apue.h"
#include "error.h"

#ifdef SOLARIS
#define PSCMD "ps -a -o pid,ppid,s,tty,com"
#else
#define PSCMD "ps -o pid,ppid,state,tty,command"
#endif

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid == 0)
    {
        exit(0);
    }

    sleep(4);
    system(PSCMD);

    exit(0);
}