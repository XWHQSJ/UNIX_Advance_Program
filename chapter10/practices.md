# 习题

## 第十章

10.1 当程序第一次接收到发送给它的信号时就终止了.因为一捕捉到信号,pause函数就返回.

10.2 见源程序 sig2str.c

10.3 图略

10.4 alarm 和 setjmp 之间存在时间窗口，如果在此期间被内核阻塞，SIGALRM 信号发送过来时，仍然没有调用过 setjmp 完成 env_alrm 的初始化，那么其行为是未定义的,改进如下:
    signal(SIGALRM, sig_arlm);
    sigset_t new_mask, old_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);

    /* 阻塞 SIGALRM */
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    alarm(60);
    if (setjmp(env_alrm) != 0) {

	    /* 解除阻塞 SIGALRM */
	    sigprocmask(SIG_SETMASK, &old_mask, NULL);
	    /* handle timeout */
    }

10.5 见pdf文件 Implementing Software Timers.pdf

10.6