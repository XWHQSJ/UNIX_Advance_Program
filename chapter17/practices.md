# 习题

## 第17章

17.1 常规管道提供了一个字节流接口，为了确定消息边界，我们必须给每个消息增加一个消息头部来指示长度。但这个仍然涉及两个额外的复制操作，一个是写入至管道，另一个是从管道读出。更加有效的方法是仅将管道用于告知主线程有一个消息可用。我们用单个字节用作通知。采用这种方法，我们需要移动mymesg结构到threadinfo结构，并使用一个互斥量(mutex)和一个条件变量(condition variable)来防止辅助线程在主线程完成之前重新使用mymesg结构。

​		详细见源代码 XSI_pipe.c

17.2 略

17.3 声明指定了标识符集合的属性（如数据类型）。如果声明也导致分配了存储单元，那么这就是定义。在头文件opend.h中，我们用extern存储类声明了3个全局变量，这时并没有为他们分配存储单元。在文件main.c中，我们定义了3个全局变量。有时，我们也会在定义全局变量时就初始化它，但通常实在使用C的默认值。

17.4 略

17.5 select和poll返回就绪的描述符个数作为函数值。当将这些就绪描述符都处理完后，操作client数组的循环就可以终止。

17.6 建议的解决方案存在的第一个问题是，在文件可能发生变化的地方，调用stat和调用unlink之间存在竞争。第二个问题是，如果名字是一个指向UNIX域套接字文件的符号链接，那么stat会报告名字是一个套接字，但是调用unlink时，实际上我们是删除了这个符号链接而不是套接字文件。为了解决第二个问题，应该使用lstat而不是stat，但这解决不了第一个问题。

17.7 第一种选择是将两个文件描述符在一个控制消息中的发送，每个文件描述符存储在相邻的内存位置中。如下代码所示：

```c
struct msghdr msg;
struct cmsghdr *cmptr;
int *ip;
if((cmptr = calloc(1, CMSG_LEN(2*sizeof(int)))) == NULL)
{
    err_sys("calloc error");
}
msg.msg_control = cmptr;
msg.msg_controllen = CMSG_LEN(2*sizeof(int));
cmptr->cmsg_len = CMSG_LEN(2*sizeof(int));
cmptr->cmsg_level = SOL_SOCKET;
cmptr->cmsg_type = SCM_RIGHTS;
ip = (int *)CMSG_DATA(cmptr);
*ip++ = fd1;
*ip = fd2;
```

第二种选择是将两个独立的cmsghdr结构打包到一个消息中，代码如下：

```c
struct msghdr msg;
struct cmsghdr *cmptr;
if((cmptr = calloc(1, CMSG_LEN(2*sizeof(int)))) == NULL)
{
    err_sys("calloc error");
}
msg.msg_control = cmptr;
msg.msg_controllen = CMSG_LEN(2*sizeof(int));
cmptr->cmsg_len = CMSG_LEN(2*sizeof(int));
cmptr->cmsg_level = SOL_SOCKET;
cmptr->cmsg_type = SCM_RIGHTS;
*(int*)CMSG_DATA(cmptr) = fd1;
cmptr->cmsg_len = CMSG_LEN(2*sizeof(int));
cmptr->cmsg_level = SOL_SOCKET;
cmptr->cmsg_type = SCM_RIGHTS;
*(int*)CMSG_DATA(cmptr) = fd2;
```

