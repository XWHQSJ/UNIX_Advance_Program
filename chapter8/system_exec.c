#include "apue.h"
#include "error.h"
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int system(const char *cmdstring)
{
    pid_t pid;
    int status;

    if (cmdstring == NULL)
    {
        return(1);
    }

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid == 0)
    {
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    }
    else
    {
        while(waitpid(pid, &status, 0) < 0)
        {
            if (errno != EINTR)
            {
                status = -1;
                break;
            }
        }
    }
    
    return (status);
}

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

int main(int argc, char *argv[])
{
    int status;

    if (argc < 2)
    {
        err_quit("command-line argument required\n");
    }
    
    if ((status = system(argv[1])) < 0)
    {
        err_sys("system() error\n");
    }

    pr_exit(status);

    exit(0);
}