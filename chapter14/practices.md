# 习题

## 第十四章

14.1 见源程序 record_lock.c 记录锁的行为是相同的,后增加的读锁可使写者不断等待,直至饿死.

14.2 大多数系统将数据类型fd_set 定义为只包含一个成员的结构,该成员为一个长整型数组.数组中每一位(bit)对应一个描述符.4个FD_宏通过开/关或测试指定的位对这个数组进行操作.将之定义为一个包含数组的结构而不仅仅时一个数组的原因是:通过C语言的赋值语句,可以使fd_set类型的变量相互赋值.

14.3 大多数系统允许用户在包括头文件 <sys/select.h> 前定义常量FD_SETSIZE.例如可以这样包含2048个描述符:
     #define FD_SETSIZE 2048
     #include <sys/select.h>
但这样做在现代系统中是不行的,需要做以下几件事:
    (1) 在包含任何头文件之前,需要定义哪种符号来防止包含<sys/select.h>;
    (2) 有时为了和旧应用兼容,<sys/types.h>定义了fd_set的大小,所以必须首先包含它们,然后去掉FD_SETSIZE的定义;
    (3) 想能够使用select时,需要重新定义FD_SETSIZE来最大化文件描述符的数量;
    (4) 需要取消定义第一步定义的符号;
    (5) 能够包含<sys/select.h>

14.4 下面列出功能类似的函数:
        FD_ZERO     sigemptyset
        FD_SET      sigaddset
        FD_CLR      sigdelset
        FD_ISSET    sigismember
没有与sigfillset对应的FD_xxx函数.对信号集来说,指向信号集的指针总是第一个参数,信号编号是第二个参数.对于描述符来说,描述符编号是第一个参数,指向描述符集的指针是第二个参数.

14.5 见源程序 select_sleep_us.h && poll_sleep_us.h

14.6 不行,可以使TELL_WAIT创建一个临时文件,其中一个字节用做父进程的锁,另外一个字节用做子进程的锁.WAIT_CHILD使得父进程等待获取子进程字节上的锁,TELL_PARENT使得子进程释放子进程字节上的锁.但问题在于,调用fork会释放所以子进程的锁,使得子进程开始运行时不具有任何它自己的锁.

14.7 见源程序 pipe_nonblock.c

14.8 