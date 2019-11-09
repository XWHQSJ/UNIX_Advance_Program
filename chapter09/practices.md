# 习题

## 第九章

9.1 因为init是登录shell的父进程，当登陆shell终止时，它收到SIGCHILD信号量，所以init进程知道什么时候终端用户注销。网络登陆没有包含init，在utmp和wtmp文件中的登录项和相应的注销项是由一个处理登陆并检测注销的进程写的。

9.2 见源程序fork_session.c