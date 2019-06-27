#include "apue.h"
#include "error.h"
#include <sys/wait.h>

char *env_init[] = {"USER=unknown", "PATH=/tmp", NULL};

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if(pid == 0)
    {
        if (execle("/home/wanhui/bin/echoall", "echoall", "myarg1", "MY ARG2", (char *)0, env_init) < 0)
        {
            err_sys("execle error\n");
        }
    }

    if (waitpid(pid, NULL, 0) < 0)
    {
        err_sys("wait error\n");
    }

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid == 0)
    {
        if (execlp("echoall", "echoall", "only 1 arg", (char *)0) < 0)
        {
            err_sys("execlp error\n");
        }
    }

    exit(0);
}