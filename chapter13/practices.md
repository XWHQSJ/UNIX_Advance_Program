# 习题

## 第十三章

13.1 如果进程调用chroot,它就不能打开/dev/log.解决的办法是,守护进程在调用chroot之前调用选项为LOG_NDELAY的openlog.它打开特殊设备文件并生成一个描述符,即使调用了chroot之后,该描述符仍然是有效的.这种场景在诸如ftpd等守护进程中出现,为安全考虑,专门调用了chroot,但仍需要调用syslog来对出错条件记录日志. 

13.2 略.

13,3 略.

13,4 见源程序 daemonize_login.c