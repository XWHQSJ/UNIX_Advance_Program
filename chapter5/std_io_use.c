#include "apue.h"
#include "error.h"
#include <stdio.h>
#include <unistd.h>
#include <libio.h>
#include <stdlib.h>

void pr_stdio(const char *, FILE *);
int buffer_size(FILE *);

int main(void)
{
    FILE *fp;
    fputs("enter any character\n", stdout);
    if (getchar() == EOF)
    {
        fprintf(stderr, "Getchar error\n");
        exit(1);
    }
    fputs("one line to standard error\n", stderr);

    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);

    fp = fopen("/etc/passwd", "r");

    if (NULL == fp)
    {
        err_sys("fopen error\n");
    }

    if (EOF == getc(fp))
    {
        err_sys("getc error\n");
    }

    pr_stdio("/etc/passwd", fp);
    exit(0);
}

void pr_stdio(const char *name, FILE *fp)
{
    printf("stream = %s,  ", name);

    if (fp->_IO_file_flags & _IO_UNBUFFERED)
    {
        printf("unbuffered");
    }
    else if (fp->_IO_file_flags & _IO_LINE_BUF)
    {
        printf("line buffered");
    }
    else
    {
        printf("fully buffered");
    }

    printf(", buffer size = %d\n", buffer_size(fp));
}

int buffer_size(FILE *fp)
{
    return (fp->_IO_buf_end - fp->_IO_buf_base);
}