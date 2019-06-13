#include "apue.h"
#include "error.h"
#include <dirent.h>
#include <limits.h>

// function type that is called for each filename
typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char *argv[])
{
    int ret;
    if (argc != 2)
    {
        err_quit("usage: ftw <starting-pathname>");
    }

    // does it all
    ret = myftw(argv[1], myfunc);

    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;

    if (ntot == 0)
    {
        // avoid divide by 0
        // print 0 for all counts
        ntot = 1;

        printf("regular files = %7ld, %5.2f %%\n", nreg, nreg * 100.0/ntot);
        printf("directories = %7ld, %5.2f %%\n", ndir, ndir * 100.0/ntot);
        printf("block special = %7ld, %5.2f %%\n", nblk, nblk * 100.0/ntot);
        printf("char special = %7ld, %5.2f %%\n", nchr, nchr * 100.0/ntot);
        printf("FIFO = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0/ntot);
        printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0/ntot);
        printf("sockets = %7ld, %5.2f %%\n", nsock, nsock * 100.0/ntot);
    }

    exit(ret);
}

// descend through the hierarchy, starting at "pathname"
// the caller's func() is called for every file.

#define FTW_F 1                 // file other than directory
#define FTW_D 2                 // directory
#define FTW_DNR 3               // directory that can't be read
#define FTW_NS 4                // file that we can't stat

// contains full pathname for every file
static char *fullpath;
static size_t pathlen;

// we return whatever func() returns
static int myftw(char *pathname, Myfunc *func)
{
    fullpath = path_alloc(&pathlen);        // malloc PATH_MAX + 1 bytes

    if (pathlen <= strlen(pathname))
    {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
        {
            err_sys("realloc failed");
        }

        strcpy(fullpath, pathname);

        return (dopath(func));
    }
}

// descend through the hierarchy, starting at "fullpath"
// if "fullpath" is anything other than a directory, we istat() it,
// call func(), and return, For a directory, we call ourself
// recursively for each name in the directory
static int dopath(Myfunc *func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret, n;

    // stat error
    if (lstat(fullpath, &statbuf) < 0)
    {
        return (func(fullpath, &statbuf, FTW_NS));
    }

    // not a directory
    if (S_ISDIR(statbuf.st_mode) == 0)
    {
        return (func(fullpath, &statbuf, FTW_F));
    }

    // it's a directory, first call func() for the directory,
    // then process each filename in the directory.
    if((ret = func(fullpath, &statbuf, FTW_D)) != 0)
    {
        return (ret);
    }

    n = strlen(fullpath);
    
    // expand path buffer
    if(n + NAME_MAX + 2 > pathlen)
    {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
        {
            err_sys("realloc failed");
        }
    }

    fullpath[n++] = '/';
    fullpath[n] = 0;

    // can't read directory
    if ((dp = opendir(fullpath)) == NULL)
    {
        return (func(fullpath, &statbuf, FTW_DNR));
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
        {
            // ignore dot and dot-dot
            continue;
        }

        // append name after "/"
        strcpy(&fullpath[n], dirp->d_name);

        // recursive
        if ((ret = dopath(func)) != 0)
        {
            // time to leave
            break;
        }
    }

    // erase everything from slash onward
    fullpath[n - 1] = 0;

    if (closedir(dp) < 0)
    {
        err_ret("can't close directory %s", fullpath);
    }

    return (ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    
}