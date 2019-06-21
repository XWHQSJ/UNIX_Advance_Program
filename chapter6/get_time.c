#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{   
    time_t t;
    struct tm *tmp;
    char buf[64];

    time(&t);
    tmp = localtime(&t);

    if (strftime(buf, 64, "%Y %m %d %A %T %Z", tmp) == 0)
    {
        printf("buf length 64 is too small\n");
    }
    else
    {
        printf("%s\n", buf);
    } 
    
    return 0;
}