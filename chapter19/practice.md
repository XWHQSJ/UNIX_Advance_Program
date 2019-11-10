# 习题

## 第19章

19.1 telnetd和rlogind两个服务器均以超级用户权限运行，所以它们都可以成功调用chown和chmod。

19.2 执行pty -n stty -a以避免伪终端从设备的termios结构和winsize结构初始化。

19.3 ignore

19.4 fcntl的F_SETFL命令不允许改变读写状态。

19.5 又3个进程组：（1）登录shell，（2）pty父进程和子进程，（3）cat进程。钱两个进程组组成了一个会话，其中，登录shell为会话首进程。第二个会话仅包含cat进程。第一个进程组是后台进程，其他两个进程组是前台进程组。

19.6 首先，当cat从其行规程模块接收到文件结束符时会终止。这导致PTY从此设备终止，进而导致PTY主设备终止。接着，对于正从PTY主设备读取的pty父进程产生一个文件结束符。该父进程将SIGTERM信号发送给子进程，于是子进程终止（子进程不捕捉该信号）。最后，父进程调用main函数尾端的exit(0).

19.7 可通过使用shell的echo命令和data(1)命令实现，它们都在一个shell中：

#!/bin/sh
(echo "Script startted on " 'data';
pty "$(SHELL:~/bin/sh)";
echo "Script done on " 'data' ) | tee typescript

19.8 PTY从设备上的行规程能够回显，所以pty从其标准输入所读取的以及写向PTY主设备的按默认都回显。尽管程序（ttyname）从不读取数据，但是该回显也可通过从设备上的行规程模块实现。