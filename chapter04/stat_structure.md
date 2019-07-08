struct stat {
    mode_t          st_mode;    // file type & mode (permission)
    ino_t           st_ino;     // i-node number (serial number)
    dev_t           st_dev;     // device number (file system)
    dev_t           st_rdev;    // device number for special files
    nlink_t         st_nlink;   // number of links
    uid_t           st_uid;     // user ID of owner
    gid_t           st_gid;     // group ID of owner
    off_t           st_size;    // size in bytes, for regular files
    struct timespec st_atime;   // time of last access
    struct timespec st_mtime;   // time of last modification
    struct timespec st_ctime;   // time of last file status change
    blksize_t       st_blksize; // best I/O block size
    blkcnt_t        st_blocks;  // number of disk blocks allocated
};