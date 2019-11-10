#include "apue.h"
#include "pty_open.h"
#include "pty_fork.h"
#include "pty_loop.h"
#include "pty_do_driver.h"
#include <termios.h>

#define OPTSTR "+d:einv"

static void set_noecho(int);
void do_driver(char *);
void loop(int, int);

int main(int argc, char *argv[])
{
    int fdm, c, ignoreeof, interactive, noecho, verbose;
    pid_t pid;
    char *driver;
    char slave_name[20];
    struct termios orig_termios;
    struct winsize size;

    interactive = isatty(STDIN_FILENO);
    ignoreeof = 0;
    noecho = 0;
    verbose = 0;
    driver = NULL;

    opterr = 0;
    while ((c = getopt(argc, argv, OPTSTR)) != EOF)
    {
        switch (c)
        {
        case 'd':
            driver = optarg;
            break;
        case 'e':
            noecho = 1;
            break;
        case 'i':
            ignoreeof = 1;
            break;
        case 'n':
            interactive = 0;
            break;
        case 'v':
            verbose = 1;
            break;
        case '?':
            err_quit("unrecognized option: -%c", optopt);
        }
    }

    if (optind >= argc)
    {
        err_quit("usage: pty [ -d driver -einv ] program [ arg ... ]");
    }

    if (interactive)
    {
        if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
        {
            err_sys("tcgetattr error on stdin");
        }

        if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&size) < 0)
        {
            err_sys("TIOCGWINSZ error");
        }

        pid = pty_fork(&fdm, slave_name, sizeof(slave_name), &orig_termios, &size);
    }
    else
    {
        pid = pty_fork(&fdm, slave_name, sizeof(slave_name), NULL, NULL);
    }

    if (pid < 0)
    {
        err_sys("fork error");
    }
    else if (pid == 0)
    {
        if (noecho)
        {
            set_noecho(STDIN_FILENO);
        }

        if (execvp(argv[optind], &argv[optind]) < 0)
        {
            err_sys("can't execute: %s", argv[optind]);
        }
    }

    if (verbose)
    {
        fprintf(stderr, "slave name = %s\n", slave_name);
        if (driver != NULL)
        {
            fprintf(stderr, "driver = %s\n", driver);
        }
    }

    if (interactive && driver == NULL)
    {
        if (tty_raw(STDIN_FILENO) < 0)
        {
            err_sys("tty_raw error");
        }
        if (atexit(tty_atexit) < 0)
        {
            err_sys("atexit error");
        }
    }

    if (driver)
    {
        do_driver(driver);
    }

    loop(fdm, ignoreeof);
    exit(0);
}

static void set_noecho(int fd)
{
    struct termios stermios;

    if (tcgetattr(fd, &stermios) < 0)
    {
        err_sys("tcgetattr error");
    }

    stermios.c_lflag &= ~(ECHO | ECHOK | ECHONL);

    stermios.c_oflag &= ~(ONLCR);

    if (tcsetattr(fd, TCSANOW, &stermios) < 0)
    {
        err_sys("tcsetattr error");
    }
}