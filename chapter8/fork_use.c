#include "apue.h"
#include "error.h"

// external variable in initialized data
int globvar = 6;
char buf[] = "a write to stdout\n";

int main(void)
{
    // automatic variable on this stack
    int var;
    pid_t pid;

    var = 88;
    if (write(STDERR_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
    {
        err_sys("write error\n");
    }

    // we don't flush stdout
    printf("before fork\n");

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid == 0)
    {
        // child
        globvar ++;
        var ++;
    }
    else
    {
        sleep(2);
    }

    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid, globvar, var);
    
    exit(0);
}