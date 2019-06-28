#include "apue.h"
#include <sys/acct.h>

#if !defined(HAS_ACORE)
#define ACORE 0
#endif
#if !defined(HAS_AXSIG)
#define AXSIG 0
#endif

static unsigned long compt2ulong(comp_t comptime)
{
    unsigned long val;
    int exp;

    val = comptime & 0x1fff;
    exp = (comptime >> 13) & 7;
    while(exp-- > 0)
    {
        val *= 8;
    }

    return val;
}

int main(int argc, char *argv[])
{
    struct acct acdata;
    FILE *fp;

    if (argc != 2)
    {
        err_quit("usage: pracct filename\n");
    }

    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        err_sys("can't open %s\n", argv[1]);
    }

    while (fread(&acdata, sizeof(acdata), 1, fp) == 1)
    {
        printf(FMT, )
    }

    exit(0);
}