# 习题

## 第四章

4.1 会输出符号链接所指向文件的文件类型

4.2 如果创建的是目录，则该目录的用户、组和其他均无法访问该目录，在目录下创建或删除文件，无法搜索目录下的文件；如果创建的是文件，则文件的用户、组和其他均无法读、写和执行该文件。

4.3 4.4 略

4.5 目录长度不会为0，因为它总是包含 . 和 .. 项；符号链接的长度也不会为0，因为路径名至少包含一个字符。

4.6 见源程序 cp_use.c

4.7 cat命令程序中调用了程序umask,而不是使用shell的umask来创建文件。

4.8 du命令查看的磁盘占用空间的大小，创建的新文件并没有写入内容，所以占用磁盘空间大小为0；df命令查看的是磁盘最大可用空间，即以使用数据块为单位进行统计，尽管新文件没有写入内容，但是占据了一定数量的数据块。

4.9 unlink 会修改 i 节点的引用次数，所以会修改文件状态更改时间。

4.10 用 opendir 打开一个目录后，递归函数调用dopath。假设opendir使用一个文件描述符，并且只有在处理完目录后才调用closedir释放描述符，这就意味着每次降一级就要使用另一描述符。所以进程最大描述符数就限制了我们可以遍历的文件系统树的深度。

4.11 见源程序recursive_travel_filedir_2.c

4.12 chroot函数被因特网文件传输协议（Internet File Transfer Protocol, FTP）程序用于辅助安全性。系统中没有账户的用户（也称匿名FTP）放在一个单独的目录下，利用chroot讲此目录当做新的根目录，就可以阻止用户访问此目录以外的文件。 chroot也用于另一台机器上构造一个文件系统层次结构的副本，然后修改此副本，不会更改原来的文件系统。这可用于测试新软件包的安装。 chroot只能由超级用户执行，一旦更改了一个进程的根，该进程及后代进程就再也不能恢复至原先的根。

4.13 首先调用stat函数取得文件的3个是时间值，然后调用utimes设置期望的值，最近访问utimes时我们不希望改变的值应当是stat中相应的值。

4.14 finger(1)对邮箱调用stat函数，最近一次的修改时间是上一次接收邮件的时间，最近一次访问时间是上一次读邮件的时间。

4.15 略

4.16 见源程序 getcwd_use_2.c

4.17 因为/dev目录关闭了其他用户的写权限,所以unlink(path)失败。