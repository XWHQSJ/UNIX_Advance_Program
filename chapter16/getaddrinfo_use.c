#include "apue.h"
#include "error.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

void print_family(struct addrinfo *aip)
{
    printf(" family ");
    switch (aip->ai_family)
    {
    case AF_INET:
        printf("inet");
        break;
    case AF_INET6:
        printf("inet6");
        break;
    case AF_UNIX:
        printf("unix");
        break;
    case AF_UNSPEC:
        printf("unspec");
        break;
    default:
        printf("unknown");
    }
}

void print_type(struct addrinfo *aip)
{
    printf(" type ");
    switch (aip->ai_socktype)
    {
        case SOCK_STREAM:
            printf("stream");
            break;
        case SOCK_DGRAM:
            printf("dgram");
            break;
        case SOCK_SEQPACKET:
            printf("seqpacket");
            break;
        case SOCK_RAW:
            printf("ram");
            break;
        default:
            printf("unknown (%d)", aip->ai_socktype);
    }
}

void print_protocol(struct addrinfo *aip)
{
    printf(" protocol ");
    switch(aip->ai_protocol)
    {
        case 0:
            printf("default");
            break;
        case IPPROTO_TCP:
            printf("tcp");
            break;
        case IPPROTO_UDP:
            printf("udp");
            break;
        case IPPROTO_RAW:
            printf("raw");
            break;
        default:
            printf("unknown (%d)", aip->ai_protocol);
    }
}

void print_flag(struct addrinfo *aip)
{
    printf(" flag ");
    if(aip->ai_flags == 0)
    {
        printf(" 0 ");
    }
    else
    {
        if(aip->ai_flags & AI_PASSIVE)
        {
            printf(" passive");
        }
        if(aip->ai_flags & AI_CANONNAME)
        {
            printf(" canon");
        }
        if(aip->ai_flags & AI_NUMERICHOST)
        {
            printf(" numhost");
        }
        if(aip->ai_flags & AI_NUMERICSERV)
        {
            printf(" numserv");
        }
        if(aip->ai_flags & AI_V4MAPPED)
        {
            printf(" v4mappe");
        }
        if(aip->ai_flags & AI_ALL)
        {
            printf(" all");
        }
    }
}

int main(int argc, char *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    struct sockaddr_in *sinp;
    const char *addr;
    int err;
    char abuf[INET_ADDRSTRLEN];

    if(argc != 3)
    {
        err_quit("usage: %s nodename service", argv[0]);
    }

    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = 0;
    hint.ai_socktype = 0;
    hint.ai_socktype = 0;
    hint.ai_protocol = 0;
    hint.ai_addrlen = NULL;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;

    if((err = getaddrinfo(argv[1], argv[2], &hint, &ailist)) != 0)
    {
        err_quit("getaddrinfo error: %s", gai_strerror(err));
    }

    for(aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        print_flag(aip);
        print_family(aip);
        print_type(aip);
        print_protocol(aip);
        printf("\n\thost %s", aip->ai_canonname?aip->ai_canonname:"-");

        if(aip->ai_family == AF_INET)
        {
            sinp = (struct sockaddr_in *)aip->ai_addr;
            addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
            printf(" address %s", addr?addr:"unknown");
            printf(" port %d", ntohs(sinp->sin_port));
        }
        printf("\n");
    }
    exit(0);
}