#include "../include/apue.h"

int g_var = 6;  // 初始化数据段

int main() {
    int var = 88;
    printf("before vfork\n");

    pid_t pid = vfork();
    if (pid < 0) {
        err_sys("vfork");
    } else if (pid == 0) {  // 子进程
        g_var++;
        var++;
        printf("child[%ld] sleeps for 1s\n", (long)getpid());
        sleep(1);
        _exit(0);  // 子进程退出，因此只有父进程执行后续操作
    }

    printf("parent[%ld] glob = %d, var = %d\n", (long)getpid(), g_var, var);
    return 0;
}
/* 实验结果
$ ./vfork 
before vfork
child[3926] sleeps for 1s
parent[3925] glob = 7, var = 89
*/
/** 结果分析
 * vfork的子进程对全局变量和局部变量的修改均影响了父进程，因此vfork的数据空间是共享的。
 * 同时可以看到父进程是等待子进程休眠1秒之后才打印的，因为vfork保证
 * 子进程先运行，在它调用exec或exit之后父进程才可能被调度运行。
 */
