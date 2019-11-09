#include "apue.h"
#include "error.h"

int globvar = 6;

int main(void)
{
    int var;
    pid_t pid;

    var = 88;
    printf("before vfork\n");

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid == 0)
    {
        globvar ++;
        var ++;
        _exit(0);
    }

    printf("pid = %ld, glob = %d, var = %d\n", pid, globvar, var);


    exit(0);
}