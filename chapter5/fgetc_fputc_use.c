#include "apue.h"
#include "error.h"

int main(void)
{
    int c;

    while ((c = fgetc(stdin)) != EOF)
    {
        if (fputc(c, stdout) == EOF)
        {
            err_sys("output error");
        }
    }

    if (ferror(stdin))
    {
        err_sys("input error");
    }

    exit(0);
}