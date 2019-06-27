#include "apue.h"
#include "error.h"c
#include <sys/wait.h>

void pr_exit(int status)
{
    if (WIFEXITED(status))
    {
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
    __WCOREDUMP(status) ? " (core file generated)" : "");
#else
    "");
#endif
    }
    else if (WIFSTOPPED(status))
    {
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
    }
}

int main(void)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid == 0)
    {
        // child
        exit(7);
    }

    // wait for child
    if (wait(&status) != pid)
    {
        err_sys("wait error");
    }
    // print its status
    pr_exit(status);

    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid == 0)
    {
        // child
        // generates SIGABRT
        abort();
    }

    // wait for child
    if (wait(&status) != pid)
    {
        err_sys("wait error");
    }
    pr_exit(status);

    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid == 0)
    {
        // divide by 0 generates SIGFPE
        status /= 0;
    }

    if (wait(&status) != pid)
    {
        err_sys("wait error");
    }
    pr_exit(status);

    exit(0);
}