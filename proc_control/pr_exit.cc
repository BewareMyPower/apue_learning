#include "../include/apue.h"
#include <sys/wait.h>
#include <signal.h>

int main() {
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        exit(7);

    if (wait(&status) != pid)
        err_sys("wait error");
    pr_exit(status);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        abort();  // 产生SIGABRT信号

    if (wait(&status) != pid)
        err_sys("wait error");
    pr_exit(status);
    printf("SIGABRT: %d\n", SIGABRT);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        status /= 0;  // 除以0会产生SIGFPE信号

    if (wait(&status) != pid)
        err_sys("wait error");
    pr_exit(status);
    printf("SIGFPE: %d\n", SIGFPE);

    return 0;
}
/* 实验结果
$ ./pr_exit 
normal termination, exit status = 7
abnormal termination, signal number = 6 (core file generated)
SIGABRT: 6
abnormal termination, signal number = 8 (core file generated)
SIGFPE: 8
$ ls core
ls: cannot access 'core': No such file or directory
$ ls core
ls: cannot access 'core': No such file or directory
$ vim pr_exit.cc
$ ulimit -c
0
$ ulimit -c unlimited
$ ./pr_exit
normal termination, exit status = 7
abnormal termination, signal number = 6 (core file generated)
SIGABRT: 6
abnormal termination, signal number = 8 (core file generated)
SIGFPE: 8
$ ls -l core
-rw------- 1 xyz xyz 561152 Mar 29 19:02 core
*/
/** 结果分析
 * 可以看到abort()以及除以0产生的信号和注释中一致，另外默认core文件的限制为0
 * 因此需要ulimit -c来设置产生core文件的大小。
 */
