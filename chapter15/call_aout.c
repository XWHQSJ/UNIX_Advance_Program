#include "apue.h"
#include "popen_pclose.h"
#include "error.h"
#include <sys/wait.h>

int main(void)
{
    char line[MAXLINE];
    FILE *fpin;

    if((fpin = popen("/home/wanhui/UNIX_Advance_Program/chapter15/myuclc", "r")) == NULL)
    {
        err_sys("popen error");
    }

    for(;;)
    {
        fputs("prompt> ", stdout);
        fflush(stdout);
        if(fgets(line, MAXLINE, fpin) == NULL)
        {
            break;
        }
        if(fputs(line, stdout) == EOF)
        {
            err_sys("fputs error to pipe");
        }
    }
    if(pclose(fpin) == -1)
    {
        err_sys("pclose error");
    }
    putchar('\n');
    exit(0);
}