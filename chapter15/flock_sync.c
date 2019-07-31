#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int release_lock(int fd)
{
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    return fcntl(fd, F_SETLK, &lock);
}

int req_wr_lock(int fd)
{
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    return fcntl(fd, F_SETLKW, &lock);
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("usage: %s <#file><#loop>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDWR | O_TRUNC | O_CREAT, 0644);
    if(fd < 0)
    {
        perror("open error");
        exit(1);
    }

    int n = 0;
    write(fd, &n, 4);

    int loop = atoi(argv[2]);

    pid_t pid;
    if((pid = fork()) < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid > 0)
    {
        int value;
        int nread;
        for(int i = 0; i < loop; ++i)
        {
            if(req_wr_lock(fd) < 0)
            {
                perror("lock error");
                exit(1);
            }
            else
            {
                printf("parent lock success\n");
            }

            if(nread = read(fd, &value, 4) < 0)
            {
                perror("read error");
                exit(1);
            }
            lseek(fd, 0, SEEK_SET);

            value ++;

            if(write(fd, &value, 4) < 0)
            {
                perror("write error");
                exit(1);
            }
            lseek(fd, 0, SEEK_SET);
            printf("parent write %d\n", value);
            if(release_lock(fd) < 0)
            {
                perror("parent release lock error");
                exit(1);
            }
            else
            {
                printf("parent release success\n");
            }
            sleep(1);
        }

        int status;
        if(waitpid(pid, &status, 0) < 0)
        {
            perror("waitpid error");
            exit(1);
        }
        exit(0);
    }
    else
    {
                int value;
        int nread;
        for (int i = 0; i < loop; i++)
        {
            if (req_wr_lock(fd) < 0)
            {
                perror("lock error");
                exit(1);
            }
            else
            {
                printf("child lock succ\n");
            }

            if ((nread = read(fd, &value, 4)) < 0)
            {
                perror("read error");
                exit(1);
            }
            lseek(fd, 0, SEEK_SET);

            value++;

            if (write(fd, &value, 4) < 0)
            {
                perror("write error");
                exit(1);
            }
            lseek(fd, 0, SEEK_SET);
            printf("child write %d \n", value);
            if (release_lock(fd) < 0)
            {
                perror("child release lock error");
                exit(1);
            }
            else
                printf("child release lock\n");

            sleep(1);
        }
        exit(0);
    }
    return 0;
}