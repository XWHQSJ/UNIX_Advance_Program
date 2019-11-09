# 习题

## 第十二章

12.1 输出到文件中的内容是:
        thread started...
        parent about to fork...
        preparing locks...
        parent unlocking locks...
        parent returned from fork
        thread started...
        parent about to fork...
        preparing locks...
        child unlocking locks...
        child returned from fork
    解释结果: 标准 I/O 例程是线程安全的.调用fork时,每个进程获得标准I/O数据结构的一份副本.程序运行时把标准输出定向到终端时,输出时行缓冲的,所以每打印一行时,标准I/O库就把行写到终端上.但如果把标准输出重定向到文件的话,则标准输出就是全缓冲的.当缓冲区满或者进程关闭流时,输出才会写到文件.此例中,执行fork时,缓冲区中包含了还未写的几个打印行,所以当父进程和子进程进行最后的冲洗缓冲区中的副本时,最初的复制内容就会写入文件.

12.2 见源程序 putenv.h 和 putenv_r.h

12.3 如果在信号处理程序运行时阻塞所有的信号,那么就能使函数成为异步信号安全.问题是不能知道调用的某个函数可能并没有屏蔽已经被阻塞的信号,这样通过另一个信号处理程序可能使该函数变成可重入的.

12.4 线程安全版本的getenv回调pthread库函数会处于一种中间的不一致状态.线程初始化函数会调用malloc,并在malloc中调用getenv查找环境变量MALLOC_OPTIONS的值.为了避开此问题,合理假设程序启动是单线程的,并使用一个标志来指示线程初始化已经通过getenv完成,但标志为假时,getenv版本会和不可重入版本操作一致.提供独立的初始化函数调用pthread_once,而非从getenv里面调用它.这就要求在调用getenv程序之前调用初始化函数.

12.5 如果希望在一个程序中运行另一个程序,只能通过fork来实现,比如实现shell.

12.6 见源程序 sleep_safethread.h

12.7 条件变量的实现使用互斥锁来保护它的内部结构.由于实现细节,因而通常被隐藏起来,在fork处理程序中没有可移植的方法获取或释放锁.既然在调用fork后并不能确定条件变量中的内部状态,所以在子进程中使用条件变量就是不安全的.

12.8 略.