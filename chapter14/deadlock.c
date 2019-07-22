#include "apue.h"
#include "error.h"
#include "lock_reg.h"
#include <fcntl.h>

static volatile sig_atomic_t sigflag;       // set nonzero by sig handler
static sigset_t newmask, oldmask, zeromask;

// one signal handler for SIGUSR1 and SIGUSR2
static void sig_usr(int signo)
{
    sigflag = 1;
}

static void lockabyte(const char *name, int fd, off_t offset)
{
    if(writew_lock(fd, offset, SEEK_SET, 1) < 0)
    {
        err_sys("%s: writew_lock error", name);
    }
    printf("%s: got the lock, byte %lld\n", name, (long long)offset);
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
    int fd;
    pid_t pid;

    /*
    * Create a file and write two bytes to it.
     */
    if((fd = creat("templock", FILE_MODE)) < 0)
    {
        err_sys("create error");
    }
    if(write(fd, "ab", 2) != 2)
    {
        err_sys("write error");
    }

    TELL_WAIT();
    if((pid = fork()) < 0)
    {
        err_sys("write error");
    }
    else if(pid == 0)
    {
        lockabyte("child", fd, 0);
        TELL_PARENT(getppid());
        WAIT_PARENT();
        lockabyte("child", fd, 1);
    }
    else
    {
        lockabyte("parent", fd, 1);
        TELL_CHILD(pid);
        WAIT_CHILD();
        lockabyte("parent", fd, 0);
    }
    exit(0);
}