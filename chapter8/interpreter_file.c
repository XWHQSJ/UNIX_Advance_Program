#include "apue.h"
#include "error.h"
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid == 0)
    {
        if (execl("/home/wanhui/UNIX_Advance_Program/chapter8/testinterp", "testinterp", "myarg1", "MY ARG2", (char *)0) < 0)
        {
            err_sys("execl error\n");
        }
    }

    if (waitpid(pid, NULL, 0) < 0)
    {
        err_sys("waitpid error\n");
    }

    exit(0);
}