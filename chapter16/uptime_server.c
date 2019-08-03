#include "apue.h"
#include "error.h"
#include "initserver.h"
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/resource.h>

#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

void daemonize(const char *cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit r1;
    struct sigaction sa;

    /*
    * Clear file creation mask.
     */
    umask(0);

    /*
    * Get maximum number of file descriptors
     */
    if(getrlimit(RLIMIT_NOFILE, &r1) < 0)
    {
        err_quit("%s: can't get file limit", cmd);
    }

    /*
    * Become a session leader to lose controlling TTY.
     */
    if((pid = fork()) < 0)
    {
        err_quit("%s: can't fork", cmd);
    }
    else if(pid != 0)
    {
        exit(0);
    }    
    setsid();

    /*
    * Ensure future opens won't allocate controlling TTYs.
     */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGHUP, &sa, NULL) < 0)
    {
        err_quit("%s: can't ignore SIGHUP", cmd);
    }

    if((pid = fork()) < 0)
    {
        err_quit("%s: can't fork", cmd);
    }
    else if(pid != 0)
    {
        exit(0);
    }

    /*
    * Change the current working directory to the root so
    * we won't prevent file systems from being unmounted.
     */
    if(chdir("/") < 0)
    {
        err_quit("%s: can't change directory to /", cmd);
    }

    /*
    * Close all open file descriptors.
     */
    if(r1.rlim_max == RLIM_INFINITY)
    {
        r1.rlim_max = 1024;
    }

    for(i = 0; i < r1.rlim_max; ++i)
    {
        close(i);
    }

    /*
    * Attach file descriptors 0, 1, and 2 to /dev/null
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /*
    * Initialize the log file.
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if(fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}

int set_cloexec(int fd)
{
    int val;
    if((val = fcntl(fd, F_GETFD, 0)) , 0)
    {
        return(-1);
    }
    val |= FD_CLOEXEC;
    return(fcntl(fd, F_SETFD, val)); 
}

void serve(int sockfd)
{
    int clfd;
    FILE *fp;
    char buf[BUFLEN];

    set_cloexec(sockfd);
    for(;;)
    {
        if((clfd = accept(sockfd, NULL, NULL)) < 0)
        {
            syslog(LOG_ERR, "ruptimed: accept error: %s", strerror(errno));
            exit(1);
        }
        set_cloexec(clfd);
        if((fp = popen("/usr/bin/uptime", "r")) == NULL)
        {
            sprintf(buf, "error: %s\n", strerror(errno));
            send(clfd, buf, strlen(buf), 0);
        }
        else
        {
            while(fgets(buf, BUFLEN, fp) != NULL)
            {
                send(clfd, buf, strlen(buf), 0);
            }
            pclose(fp);
        }
        close(clfd);
    }
}

int main(int argc, char *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err, n;
    char *host;

    if(argc != 1)
    {
        err_quit("usage: ruptimed");
    }
    if((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
    {
        n = HOST_NAME_MAX;                      /* best guess */
    }
    if((host = malloc(n)) == NULL)
    {
        err_sys("malloc error");
    }
    if(gethostname(host, n) < 0)
    {
        err_sys("gethostname error");
    }

    daemonize("ruptimed");
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;

    if((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
    {
        syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s", gai_strerror(err));
        exit(1);
    }
    for(aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        if((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
        {
            serve(sockfd);
            exit(0);
        }
    }
    exit(1);
}