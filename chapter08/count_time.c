#include "apue.h"
#include "error.h"
#include <sys/times.h>

static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

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
    int i;

    setbuf(stdout, NULL);
    for (i = 1; i < argc; ++i)
    {
        do_cmd(argv[i]);
    }

    exit(0);
}

static void do_cmd(char *cmd)
{
    struct tms tmsstart, tmsend;
    clock_t start, end;
    int status;

    printf("\ncommand: %s\n", cmd);

    if ((start = times(&tmsstart)) == -1)
    {
        err_sys("times error\n");
    }

    if ((status = system(cmd) < 0))
    {
        err_sys("system() error\n");
    }

    if ((end = times(&tmsend)) == -1)
    {
        err_sys("times error\n");
    }

    pr_times(end-start, &tmsstart, &tmsend);
    pr_exit(status);
}

static void pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
    static long clktck = 0;

    if (clktck == 0)
    {
        if((clktck = sysconf(_SC_CLK_TCK)) < 0)
	{
	    err_sys("sysconf error\n");
	}
    }

    printf(" real: %7.2f\n", real / (double)clktck);
    printf(" user: %7.2f\n", (tmsend->tms_utime - tmsstart->tms_utime) / (double)clktck);
    printf(" sys: %7.2f\n", (tmsend->tms_stime - tmsstart->tms_stime / (double)clktck));
    printf(" child user: %7.2f\n", (tmsend->tms_cutime - tmsstart->tms_cutime) / (double)clktck);
    printf(" child sys: %7.2f\n", (tmsend->tms_cstime - tmsstart->tms_cstime) / (double)clktck);
}
