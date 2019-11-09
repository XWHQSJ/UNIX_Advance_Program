#include <time.h>
#include <limits.h>
#include "apue.h"

int main(void)
{
    time_t t;
    struct tm *tmp;
    tmp = NULL;
    char buf[64] = {0};

    if (tmp = localtime(&t))
    {
        strftime(buf, 63, "%Y/%m/%e %H:%M:%S", tmp);
    }

    printf("%s\n", buf);

    return 0;
}
