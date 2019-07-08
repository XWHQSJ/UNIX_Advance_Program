#include "apue.h"
#include "error.h"

void setbuf(FILE *fp, char *restrict_buf)
{
    if (restrict_buf == NULL)
    {
        setvbuf(fp, restrict_buf, _IONBF, 0);
        printf("No buf");
    }
    else
    {
        if (fp == stdin)
        {
            if (setvbuf(fp, restrict_buf, _IOLBF, BUFSIZ) != 0)
            {
                err_sys("setvbuf error");
                exit(-1);
            }
        }
        else if (fp == stdout)
        {
            if (setvbuf(fp, restrict_buf, _IOLBF, BUFSIZ) != 0)
            {
                err_sys("setvbuf error");
                exit(-1);
            }
        }
        else if (fp == stderr)
        {
            if (setvbuf(fp, restrict_buf, _IOLBF, BUFSIZ) != 0)
            {
                err_sys("setvbuf error");
                exit(-1);
            }
        }
        else
        {
            if (setvbuf(fp, restrict_buf, _IOLBF, BUFSIZ) != 0)
            {
                err_sys("setvbuf error");
                exit(-1);
            }
        }
        printf("linebuf");
    }
}