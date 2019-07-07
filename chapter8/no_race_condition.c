#include "apue.h"
#include "error.h"

static volatile sig_atomic_t sigflag;       // set nonzero by sig handler
static void charatatime(char *);
static sigset_t newmask, oldmask, zeromask;

// one signal handler for SIGUSR1 and SIGUSR2
static void sig_usr(int signo)
{
    sigflag = 1;
}

void TELL_WAIT(void)
{
    if(signal(SIGUSR1, sig_usr) == SIG_ERR)
    {
        err_sys("signal(SIGUSR1) error\n");
    }

    if(signal(SIGUSR2, sig_usr) == SIG_ERR)
    {
        err_sys("signal(SIGUSR2) error\n");
    }

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    /* Block SIGUSR1 and SIGUSR2, and save current signal mask */
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        err_sys("SIG_BLOCK error\n");
    }
}

void TELL_PARENT(pid_t pid)
{
    // tell parent we're done
    kill(pid, SIGUSR2);
}

void WAIT_PARENT(void)
{
    while(sigflag == 0)
    {
        sigsuspend(&zeromask);  // and wait for parent
    }

    sigflag = 0;

    /* Reset signal mask to original value */
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        err_sys("SIG_SETMASK error\n");
    }
}

void TELL_CHILD(pid_t pid)
{
    // tell child we're done
    kill(pid, SIGUSR1);
}

void WAIT_CHILD(void)
{
    while(sigflag == 0)
    {
        sigsuspend(&zeromask);  // add wait for child
    }
    sigflag = 0;

    /* Reset signal wait mask to original value */
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        err_sys("SIG_SETMASK error\n");
    }
}

int main(void)
{
    pid_t pid;

    TELL_WAIT();

    if ((pid = fork()) < 0)
    {
        err_sys("fork error\n");
    }
    else if (pid == 0)
    {
        // parent goes first
        WAIT_PARENT();

        charatatime("output from child\n");
    }
    else
    {
        charatatime("output from parent\n");

        TELL_CHILD(pid);
    }
    
    exit(0);
}

static void charatatime(char *str)
{
    char *ptr;
    int c;

    // set unbuffered
    setbuf(stdout, NULL);

    for(ptr = str; (c = *ptr++) != 0; )
    {
        putc(c, stdout);
    }
}