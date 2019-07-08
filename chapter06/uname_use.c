#include "apue.h"
#include "error.h"
#include <stdio.h>
#include <sys/utsname.h>

int main(void)
{
    struct utsname name;
    if (uname (&name) < 0)
    {
        err_sys("uname failed");
    }

    printf("sysname is %s\n", name.sysname);
    printf("nodename is %s\n", name.nodename);
    printf("release is %s\n", name.release);
    printf("version is %s\n", name.version);
    printf("machine is %s\n", name.machine);
    
    return 0;
}