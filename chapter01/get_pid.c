#include "apue.h"
#include "error.h"

int
main(void)
{
    printf("hello world from process ID %ld\n", (long)getgid());
    exit(0);
}