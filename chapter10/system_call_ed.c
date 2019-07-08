#include "apue.h"
#include "error.h"

static void sig_chld(int signo)
{
    printf("caught SIGCHLD\n");
}

static void sig_int(int signo)
{
    printf("caught SIGINT\n");
}

int main(void)
{
    if(signal(SIGINT, sig_int) == SIG_ERR)
    {
        err_sys("signal(SIGINT) error\n");
    }

    if(signal(SIGCHLD, sig_chld) == SIG_ERR)
    {
        err_sys("signal(SIGCHLD) error\n");
    }

    if(system("/bin/ed") < 0)
    {
        err_sys("system() error\n");
    }

    exit(0);
}