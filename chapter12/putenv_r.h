#include <limits.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

extern char **environ;

pthread_once_t init_done = PTHREAD_ONCE_INIT;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void thread_init(void)
{
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&lock, &attr);
    pthread_mutexattr_destroy(&attr);
}

int putenv_r(char *str, char *buf, int buflen, char **new_env, int new_env_len)
{
    char *eqloc;
    int exist, i, oval_len, nval_len, nname_len, oname_len, osize;
    char **new_environ;
    char *testp;

    eqloc = strstr(str, "=");
    if(eqloc == NULL)
    {
        return EINVAL;
    }

    nname_len = eqloc - str;
    eqloc ++;
    nval_len = strlen(eqloc);

    pthread_once(&init_done, thread_init);
    pthread_mutex_lock(&lock);

    for(i = 0; environ[i] != NULL; ++i)
    {
        if(strncmp(str, environ[i], nname_len) == 0 && (environ[i][nname_len] == '='))
        {
            oval_len = strlen(&environ[i][nname_len + 1]);
            if(nval_len <= oval_len)
            {
                strncpy(&environ[i][nname_len + 1], eqloc, nval_len);
                environ[i][nname_len + 1 + nval_len] = 0;
            }
            else
            {
                if(strlen(str) > buflen)
                {
                    pthread_mutex_unlock(&lock);
                    return ENOSPC;
                }
                strcpy(buf, str);
                environ[i] = buf;
            }
            pthread_mutex_unlock(&lock);
            return(0);
        }
    }

    osize = ++i;
    if(osize > new_env_len || strlen(str) > buflen)
    {
        pthread_mutex_unlock(&lock);
        return ENOSPC;
    }

    for(i = 0; i < osize; ++i)
    {
        new_environ[i] = environ[i];
    }

    strcpy(buf, str);
    new_environ[i - 1] = buf;
    new_environ[i] = NULL;
    environ = new_environ;

    pthread_mutex_unlock(&lock);
    return(0);
}