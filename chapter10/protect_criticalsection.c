#include "apue.h"
#include "error.h"

static void sig_int(int);

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
    sigset_t newmask, oldmask, waitmask;

    pr_mask("program start: ");

    if(signal(SIGINT, sig_int) == SIG_ERR)
    {
        err_sys("signal(SIGINT) error\n");
    }

    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

    /*
    * Block SIGINT and save current signal mask.
     */
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        err_sys("SIG_BLOCK error\n");
    }

    /*
    * Critical region of code
     */
    pr_mask("in critical region: ");

    /*
    * Pause, allowing all signals except SIGUSR1
     */
    if(sigsuspend(&waitmask) != -1)
    {
        err_sys("sigsuspend error\n");
    }

    pr_mask("after return from sigsuspend: ");

    /*
    * Reset signal mask which unblocks SIGINT.
     */
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        err_sys("SIG_SETMASK error\n");
    }

    /*
    * and continue processing...
     */
    pr_mask("program exit: ");
    
    exit(0);
}

static void sig_int(int signo)
{
    pr_mask("\nin sig_int: ");
}