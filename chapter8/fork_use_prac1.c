#include "apue.h"
#include "error.h"

// external variable in initialized data
int globvar = 6;
#define SIZE 256
// char buf[] = "a write to stdout\n";

int main(void)
{
    // automatic variable on this stack
    int var;
    pid_t pid;
    int i;
    char buf[SIZE];

    var = 88;

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
        fclose(stdout);
        exit(0);
    }
    else
    {
        sleep(2);
    }

    i = printf("pid = %ld, glob = %d, var = %d\n", (long)getpid, globvar, var);
    sprintf(buf, "%d\n", i);
    write(STDOUT_FILENO, buf, strlen(buf));

    exit(0);
}