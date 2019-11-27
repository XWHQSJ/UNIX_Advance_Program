# UNIX_Advance_Program

Unix Advanced Environment Programming Coding

### 文件结构
```
.
├── chapter01
│   ├── error_msg.c
│   ├── getc_stdio.c
│   ├── get_pid.c
│   ├── list_file.c
│   ├── practices.md
│   ├── print_id.c
│   ├── process_control.c
│   ├── signals.c
│   └── stdin_stdout.c
├── chapter02
│   ├── basic_sys_data_type.md
│   ├── ISO_C_HEADER.md
│   ├── open_max2.c
│   ├── open_max.c
│   ├── path_alloc.c
│   ├── practices.md
│   └── sysconf_pathconf.c
├── chapter03
│   ├── create_hole_file.c
│   ├── descriptor_fd.c
│   ├── dup2_no_fcntl.c
│   ├── file.hole
│   ├── multi_read_write.c
│   ├── offset_stdin.c
│   ├── practices.md
│   ├── read_write.c
│   └── temp.foo
├── chapter04
│   ├── access_use.c
│   ├── access_use.c.copy
│   ├── chdir_use.c
│   ├── chmod_use.c
│   ├── cp_use.c
│   ├── file_type.c
│   ├── futimens_use.c
│   ├── getcwd_use_2.c
│   ├── getcwd_use.c
│   ├── practices.md
│   ├── recursive_travel_filedir_2.c
│   ├── recursive_travel_filedir.c
│   ├── stat_structure.md
│   ├── st_dev_st_rdev.c
│   ├── umask_use.c
│   └── unlink_use.c
├── chapter05
│   ├── fgetc_fputc_use.c
│   ├── fgets_fputs_use.c
│   ├── fmemopen_use.c
│   ├── getc_putc_use.c
│   ├── mkstemp_use.c
│   ├── practices.md
│   ├── setvbuf_use.c
│   ├── std.err
│   ├── std_io_use.c
│   ├── std.out
│   └── tmpnam_tmpfile_use.c
├── chapter06
│   ├── getpwnam.c
│   ├── get_time.c
│   ├── practices.md
│   ├── shadow_look.c
│   ├── strftime_use.c
│   ├── time_t_use.c
│   └── uname_use.c
├── chapter07
│   ├── argv_ret.c
│   ├── atexit_use.c
│   ├── cmd_handle.c
│   ├── hello_world.c
│   ├── practices.md
│   ├── resource_limit.c
│   ├── setjmp_longjmp_use.c
│   └── testjmp.c
├── chapter08
│   ├── change_nice.c
│   ├── child.c
│   ├── count_time.c
│   ├── dead_process.c
│   ├── echo_all.c
│   ├── echoarg.c
│   ├── exec_use.c
│   ├── exit_status.c
│   ├── fork_fork_use.c
│   ├── fork_use.c
│   ├── fork_use_prac1.c
│   ├── interpreter_file.c
│   ├── no_race_condition.c
│   ├── opendir_info.c
│   ├── practices.md
│   ├── print_process_account.c
│   ├── printuids.c
│   ├── process_account.c
│   ├── race_condition.c
│   ├── system_exec.c
│   ├── system_use.c
│   ├── temp.out
│   ├── testinterp.sh
│   ├── vfork_error.c
│   └── vfork_use.c
├── chapter09
│   ├── fork_session.c
│   ├── orphan_process.c
│   └── practices.md
├── chapter10
│   ├── abort_use.h
│   ├── call_self_system.c
│   ├── catch_sigusr.c
│   ├── fwrite.c
│   ├── handle_SIGTSTP.c
│   ├── Implementing Software Timers.pdf
│   ├── my_alarm.c
│   ├── practices.md
│   ├── pr_mask.c
│   ├── protect_criticalsection.c
│   ├── sig2str.c
│   ├── sigaction_to_signal.c
│   ├── sigaddset_sigdelset_sigismember.h
│   ├── sigcld_use.c
│   ├── signal_intr.c
│   ├── sigprocmask_use.c
│   ├── sigsetjmp_siglongjmp_use.c
│   ├── sigsuspend_wait.c
│   ├── sigxfsz.c
│   ├── sleep2_use.c
│   ├── sleep.h
│   ├── sync.c
│   ├── system_call_ed.c
│   ├── system_use.h
│   ├── temp.foo
│   ├── time.c
│   ├── timelimit_longjmp_read.c
│   └── timelimit_read.c
├── chapter11
│   ├── maketimeout.h
│   ├── practices.md
│   ├── pthread_barrier.c
│   ├── pthread_cleanup_handler.c
│   ├── pthread_condition.h
│   ├── pthread_exit_11_4.c
│   ├── pthread_exit_errarg.c
│   ├── pthread_exit_status.c
│   ├── pthread_id.c
│   ├── pthread_mutex.h
│   ├── pthread_mutex_simple.h
│   ├── pthread_mutex_timedlock.c
│   ├── pthread_rdlock.h
│   └── pthread_two_mutex.h
├── chapter12
│   ├── foo
│   ├── getenv.h
│   ├── getenv_safethread2.h
│   ├── getenv_safethread.h
│   ├── practices.md
│   ├── pthread_atfork.c
│   ├── pthread_detachstate.h
│   ├── pthread_recursive.c
│   ├── putenv.h
│   ├── putenv_r.h
│   ├── signal_sync.c
│   └── sleep_safethread.h
├── chapter13
│   ├── daemonize_init.h
│   ├── daemonize_login.c
│   ├── daemonize_rereadconf2.c
│   ├── daemonize_rereadconf.c
│   ├── daemonize_single.h
│   ├── practices.md
│   └── set_cloexec.h
├── chapter14
│   ├── clr_fl.h
│   ├── deadlock.c
│   ├── enforcement-mode_lock.c
│   ├── lockfile
│   ├── lockfile.h
│   ├── lock_reg.h
│   ├── lock_test.h
│   ├── mmap_dupfile.c
│   ├── pipe_nonblock.c
│   ├── poll_sleep_us.h
│   ├── practices.md
│   ├── readn_writen.h
│   ├── record_lock.c
│   ├── ROT-13.c
│   ├── ROT_sync.c
│   ├── select_sleep_us.h
│   ├── set_fl.h
│   ├── stderr.out
│   ├── temp.foo
│   ├── templock
│   ├── temp.lock
│   ├── temp.out
│   └── write_nonblock.c
├── chapter15
│   ├── add2.c
│   ├── add2_std.c
│   ├── call_aout.c
│   ├── clr_fl.h
│   ├── coprocess_add2.c
│   ├── coprocess_add2_std.c
│   ├── data_location.c
│   ├── flock_sync.c
│   ├── ipc_devzero.c
│   ├── nonblock_fifo.c
│   ├── pipe_dupfile.c
│   ├── pipe_parenttochild.c
│   ├── popen_dupfile.c
│   ├── popen_pclose.h
│   ├── practices.md
│   ├── semaphore_mutex.h
│   ├── sem_sync.c
│   ├── sync_parentchild.h
│   └── uppertolower.c
├── chapter16
│   ├── connect_retry2.h
│   ├── connect_retry.h
│   ├── getaddrinfo_use.c
│   ├── initserver2.h
│   ├── initserver.h
│   ├── practices.md
│   ├── print_byteorder.c
│   ├── socket_async.h
│   ├── uptime_client_dgram.c
│   ├── uptime_client_stream.c
│   ├── uptime_server2_stream.c
│   ├── uptime_server_dgram.c
│   └── uptime_server_stream.c
├── chapter17
│   ├── cli_conn.h
│   ├── fd_pipe.h
│   ├── practices.md
│   ├── ProcessSendfdToFatherProcess
│   │   ├── buf_args.h
│   │   ├── cli_args.h
│   │   ├── csopen.h
│   │   ├── handle_request.h
│   │   ├── main.c
│   │   ├── opend.h
│   │   ├── open.h
│   │   └── server_main.c
│   ├── ProcessSendfdToFatherProcessV2
│   │   ├── clients_v2.c
│   │   ├── csopen_v2.h
│   │   ├── handle_request_v2.h
│   │   ├── loop_poll.h
│   │   ├── loop_select.h
│   │   ├── main_v2.c
│   │   ├── opend_v2.h
│   │   ├── open_v2.h
│   │   └── server_main_v2.c
│   ├── send_err.h
│   ├── sendmsg.c
│   ├── serv_accept.h
│   ├── serv_listen.h
│   ├── socket_bind.c
│   ├── socket_pollmsg.c
│   ├── socket_recv_fd2.h
│   ├── socket_recv_fd.h
│   ├── socket_send_fd2.h
│   ├── socket_send_fd.h
│   └── XSI_pipe.c
├── chapter18
│   ├── cbreak_main.c
│   ├── ctermid.h
│   ├── getpass_main.c
│   ├── isatty_main.c
│   ├── no_intr.c
│   ├── practices.md
│   ├── print_win_size.c
│   ├── tcgetattr_tcsetattr.c
│   └── ttyname_main.c
├── chapter19
│   ├── practice.md
│   ├── pty_do_driver.h
│   ├── pty_fork.h
│   ├── pty_loop.h
│   ├── pty_main.c
│   └── pty_open.h
├── chapter20
│   ├── apue_db.h
│   ├── db.c
│   └── practice.md
├── chapter21
└── README.md
```

### 参考内容

- Book SourceCode from: http://www.apuebook.com/
- Good Code is: https://github.com/YangXiaoHei/APUE
