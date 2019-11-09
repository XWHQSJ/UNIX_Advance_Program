#include "apue.h"
#include <signal.h>
#include <stdio.h>
#include <termios.h>

#define MAX_PASS_LEN 8 // max #chars for user to enter

char *getpass(const char *prompt)
{
    static char buf[MAX_PASS_LEN + 1]; // null byte at end
    char *ptr;
    sigset_t sig, osig;
    struct termios ts, ots;
    FILE *fp;
    int c;

    if ((fp = fopen(ctermid(NULL), "r")) == NULL)
    {
        return NULL;
    }

    setbuf(fp, NULL);

    sigemptyset(&sig);
    sigaddset(&sig, SIGINT);        // block SIGINT
    sigaddset(&sig, SIGTSTP);       // block SIGTSTP
    sigprocmask(SIG_BLOCK, &sig, &osig);

    tcgetattr(fileno(fp), &ts);     // save tty state
    ots = ts;
    ts.c_lflag &= ~(ECHO | ECHOE | ECHOK |ECHONL);
    tcsetattr(fileno(fp), TCSAFLUSH, &ts);
    fputs(prompt, fp);

    ptr = buf;
    while((c = getc(fp)) != EOF && c != '\n')
    {
        if(ptr < &buf[MAX_PASS_LEN])
        {
            *ptr++ = c;
        }
    }

    *ptr = 0;
    putc('\n', fp);

    tcsetattr(fileno(fp), TCSAFLUSH, &ots);
    sigprocmask(SIG_SETMASK, &osig, NULL);
    fclose(fp);
    return(buf);
}

int main(void)
{
    char *ptr;

    if((ptr = getpass("Enter password:")) == NULL)
    {
        err_sys("getpass error");
    }
    printf("password: %s\n", ptr);

    /* now use password */

    while (*ptr != 0)
    {
        *ptr++ = 0;
    }

    exit(0);
}