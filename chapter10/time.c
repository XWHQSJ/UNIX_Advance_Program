#include "apue.h"
#include "error.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int fd = open("test", O_CREAT | O_RDWR | O_TRUNC, 0644);

    if(fd < 0)
    {
        perror("open error");
        exit(1);
    }

    char buf[100] = {0};
    char *line = "-------------------------\n";
    while(1)
    {
        time_t c = time(NULL);
        struct tm *t = localtime(&c);
        if(t->tm_sec != 59)
        {
            snprintf(buf, sizeof(buf), "%d\n", t->tm_sec);
        }
        else
        {
            snprintf(buf, sizeof(buf), "%d\n-----------------------\n", t->tm_sec);
        }
        
        int len = strlen(buf);
        if(write(fd, buf, len) != len)
        {
            perror("write error");
            exit(1);
        }
        sleep(1);
    }

    return 0;
}