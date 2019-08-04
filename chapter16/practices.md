# 习题

## 第十六章

16.1 见源程序 print_byteorder.c

16.2 

16.3 对于我们将要监听的每个端点，需要绑定到一个合适的地址，并队友每个描述符在fd_set结构中写一条记录。然后使用select等待从多个端点来的连接请求。当一个连接请求到达时，一个被动的端点将会变得可读。当一个连接请求真的到达时，我们接受该请求，并如以前一样处理。

16.4

16.5 在main过程中，通过调用signal函数来捕捉SIGCHLD信号，该函数将使用sigaction来安装处理程序指定可重启的系统调用选项。下一步，从server函数中删除waitpid调用。当fork完子进程来处理请求后，父进程关闭新的文件描述符并继续监听新的连接请求。最后，需要一个针对于SIGCHLD的信号处理程序，如下：
    void sigchld(int signo)
    {
        while(waitpid((pid_t) - 1, NULL, WNOHANG) > 0)
            ;
    }

16.6 为了允许异步I/O，需要使用F_SETOWN fcntl命令建立套接字所有权，然后使用FIOASYNC ioctl命令允许异步信号。为了不允许异步套接字I/O，只要简单地禁用异步信号即可。混合使用fcntl和ioctl命令的理由是，想找到最多移植的方法。
    见源程序 socket_sync.h

16.7 