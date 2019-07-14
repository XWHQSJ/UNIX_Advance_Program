#include "apue.h"
#include "error.h"
#include <pthread.h>

struct foo
{
    int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp)
{
    printf("%s", s);
    printf(" structure at 0x%lx\n", (unsigned long)fp);
    printf(" foo.a = %d\n", fp->a);
    printf(" foo.b = %d\n", fp->b);
    printf(" foo.c = %d\n", fp->c);
    printf(" foo.d = %d\n", fp->d);
}

void * thr_fn1(void *arg)
{
    printfoo("thread 1:\n", arg);
    pthread_exit((void*)arg);
}

void *thr_fn2(void *arg)
{
    printf("thread 2:\n", arg);
    pthread_exit((void*)arg);
}

int main(void)
{
    struct foo foo = {1, 2, 3, 4};
    int err = 0;
    pthread_t tid1, tid2;
    struct foo *fp;

    err = pthread_create(&tid1, NULL, thr_fn1, &foo);
    if(err != 0)
    {
        err_exit(err, "can't create thread 1");
    }

    err = pthread_join(tid1, (void*)&fp);
    if(err != 0)
    {
        err_exit(err, "can't join with thread\n");
    }

    err = pthread_create(&tid2, NULL, thr_fn2, fp);
    if(err != 0)
    {
        err_exit(err, "can't create thread 2");
    }

    err = pthread_join(tid2, NULL);
    if(err != 0)
    {
        err_exit(err, "can't join with thread\n");
    }

    printfoo("main thread:\n", fp);

    exit(0);
}