#include "opend_v2.h"
#include <fcntl.h>

void handle_request(char *buf, int nread, int clifd, uid_t uid)
{
    int newfd;

    if(buf[nread -1] != 0)
    {
        snprintf(errmsg, MAXLINE - 1, "request not null terminated: %*.*s\n", nread, nread, buf);
        send_err(clifd, -1, errmsg);
        return;
    }

    log_msg("request: %s, from uid %d", buf, uid);
    
    // parse args & set options
    if(buf_args(buf, cli_args) < 0)
    {
        send_err(clifd, -1, errmsg);
        log_msg(errmsg);
        return;
    }

    if((newfd = open(pathname, oflag)) < 0)
    {
        snprintf(errmsg, MAXLINE - 1, "can't open %s: %s\n", pathname, strerror(errno));
        send_err(clifd, -1, errmsg);
        log_msg(errmsg);
        return;
    }

    // send the descriptor
    if(send_fd(clifd, newfd) < 0)
    {
        log_sys("send_fd error");
    }

    log_msg("sent fd %d over fd %d for %s", newfd, clifd, pathname);
    // we are done with descriptor
    close(newfd);
}