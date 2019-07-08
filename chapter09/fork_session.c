#include "apue.h"
#include "error.h"
#include <errno.h>
#include <unistd.h>

static void pr_ids(char *name)
{
    printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n", name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));

    fflush(stdout);
}

int main(void)
{
    char c;
    pid_t pid;

    pr_ids("parent");
    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid > 0)
    {
        sleep(5);
    }
    else
    {
        pr_ids("child");
	setsid();
	pr_ids("child");

	if (read(STDIN_FILENO, &c, 1) != 1)
	{
	    printf("read error %d on controlling TTY\n", errno);
	}
    }

    exit(0);
}
