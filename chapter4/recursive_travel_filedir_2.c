#include "apue.h"
#include "error.h"
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>

// function type that is called for each filename
typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;

// if PATH_MAX is indeterminate, no guarantee this is adequate
#define PATH_MAX_GUESS 1024

char *path_alloc(size_t *sizep)
{
    char *ptr;
    size_t size;

    if (posix_version == 0)
    {
        posix_version = sysconf(_SC_VERSION);
    }

    if (xsi_version == 0)
    {
        xsi_version = sysconf(_SC_XOPEN_VERSION);
    }

    // first time through
    if (pathmax == 0)
    {
        errno = 0;

        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0)
        {
            if (errno == 0)
            {
                // it is indeterminate
                pathmax = PATH_MAX_GUESS;
            }
            else
            {
                err_sys("pathconf error for _PC_PATH_MAX");
            }
        }
        else
        {
            pathmax++;
        }
    }

    // before POSIX.1-2001, we aren't guaranteed that PATH_MAX includes
    // the terminating null byte. Same goes for XPG3
    if ((posix_version < 200112L) && (xsi_version))
    {
        size = pathmax + 1;
    }
    else
    {
        size = pathmax;
    }

    if ((ptr = malloc(size)) == NULL)
    {
        err_sys("malloc error for pathname");
    }

    if (sizep != NULL)
    {
        *sizep = size;
    }

    return (ptr);
}

int main(int argc, char *argv[])
{
    double whole;
    clock_t str, end;
    int ret;
    str = clock();
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
    }
    
    printf("regular files = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFO = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);

    end = clock();
    whole = (double)(end - str) / CLOCKS_PER_SEC;
    printf("costs: %f second\n", whole);

    exit(ret);
}

// descend through the hierarchy, starting at "pathname"
// the caller's func() is called for every file.

#define FTW_F 1   // file other than directory
#define FTW_D 2   // directory
#define FTW_DNR 3 // directory that can't be read
#define FTW_NS 4  // file that we can't stat

// contains full pathname for every file
static char *fullpath;
static size_t pathlen;

// we return whatever func() returns
static int myftw(char *pathname, Myfunc *func)
{
    fullpath = path_alloc(&pathlen); // malloc PATH_MAX + 1 bytes

    if (pathlen <= strlen(pathname))
    {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
        {
            err_sys("realloc failed");
        }
    }

    strcpy(fullpath, pathname);

    return (dopath(func));
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
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
    {
        return (ret);
    }

    n = strlen(fullpath);

    // expand path buffer
    if (n + NAME_MAX + 2 > pathlen)
    {
        pathlen *= 2;
        printf("realloc\n");
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
    switch (type)
    {
    case FTW_F:
        switch (statptr->st_mode & S_IFMT)
        {
        case S_IFREG:
            nreg++;
            break;
        case S_IFBLK:
            nblk++;
            break;
        case S_IFCHR:
            nchr++;
            break;
        case S_IFIFO:
            nfifo++;
            break;
        case S_IFLNK:
            nslink++;
            break;
        case S_IFSOCK:
            nsock++;
            break;
        // directory should have type = FTW_D
        case S_IFDIR:
            err_dump("for S_IFDIR for %s", pathname);
        }
        break;
    case FTW_D:
        ndir++;
        break;
    case FTW_DNR:
        err_ret("can't read directory %s", pathname);
        break;
    case FTW_NS:
        err_ret("stat error for %s", pathname);
        break;
    default:
        err_dump("unknown type %d for pathname %s", type, pathname);
    }

    return 0;
}