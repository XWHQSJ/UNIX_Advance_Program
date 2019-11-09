#include "apue.h"
#include "error.h"
#include <setjmp.h>
#include <time.h>

static void sig_usr1(int);
static void sig_alrm(int);
static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;


void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;

    // we can be called by signal handlers
    errno_save = errno;
    
    if(sigprocmask(0, NULL, &sigset) < 0)
    {
        err_ret("sigprocmask error\n");
    }
    else
    {
        printf("%s", str);
        if(sigismember(&sigset, SIGINT))
        {
            printf(" SIGINT");
        }
        if(sigismember(&sigset, SIGQUIT))
        {
            printf(" SIGQUIT");
        }
        if(sigismember(&sigset, SIGUSR1))
        {
            printf(" SIGUSR1");
        }
        if(sigismember(&sigset, SIGALRM))
        {
            printf(" SIGALRM");
        }

        printf("\n");
    }

    errno = errno_save;
    
}

int main(void)
{
    if(signal(SIGUSR1, sig_usr1) == SIG_ERR)
    {
        err_sys("signal(SIGUSR1) error\n");
    }

    if(signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        err_sys("signal(SIGALRM) error\n");
    }

    pr_mask("starting main: ");

    if(sigsetjmp(jmpbuf, 1))
    {
        pr_mask("ending main: ");

        exit(0);
    }

    // now sigsetjmp() is OK
    canjump = 1;

    for(;;)
    {
        pause();
    }
}

static void sig_usr1(int signo)
{
    time_t starttime;

    if(canjump == 0)
    {
        return;
    }

    pr_mask("starting sig_usr1: ");

    // SIGALRM in 3 seconds
    alarm(3);
    starttime = time(NULL);

    for(;;)
    {
        if(time(NULL) > starttime + 5)
        {
            break;
        }
    }

    pr_mask("finishing sig_usr1: ");

    canjump = 0;
    // jump back to main, don't return
    siglongjmp(jmpbuf, 1);
}

static void sig_alrm(int signo)
{
    pr_mask("in sig_alrm: ");
}