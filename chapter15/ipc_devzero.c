#include "apue.h"
#include "error.h"
#include <fcntl.h>
#include <sys/mman.h>

#define NLOOPS  1000
#define SIZE    sizeof(long)    /* size of shared memory area */

static int update(long *ptr)
{
    return ((*ptr)++);          /* return value before increment */
}

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
    int fd, i, counter;
    pid_t pid;
    void *area;

    if((fd = open("/dev/zero", O_RDWR)) < 0)
    {
        err_sys("open error");
    }

    if((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        err_sys("mmap error");
    }
    close(fd);

    TELL_WAIT();

    if((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if(pid > 0)            /* parent */
    {
        for(i = 0; i < NLOOPS; i += 2)
        {
            if((counter = update((long *)area)) != i)
            {
                err_quit("parent: expected %d, got %d", i, counter);
            }

            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    }
    else
    {
        for(i = 0; i < NLOOPS + 1; i += 2)
        {
            WAIT_PARENT();

            if((counter = update((long *)area)) != i)
            {
                err_quit("child: expected %d, got %d", i, counter);
            }
            TELL_PARENT(getppid());
        }
    }
    exit(0);
}