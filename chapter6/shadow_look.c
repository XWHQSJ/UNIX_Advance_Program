#include "apue.h"
#include <stdio.h>
#include <shadow.h>
#include <string.h>

int main(void)
{
    struct spwd *ptr;

    while ((ptr = getspent()) != NULL)
    {
        if (strcmp(ptr->sp_namp, "wanhui") == 0)
        {
            break;
        }
    }

    printf("%s\n", ptr->sp_pwdp);

    return 0;
}