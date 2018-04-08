#include "../include/apue.h"
#include <setjmp.h>

constexpr int MAXLINE = 1024;
static jmp_buf env_alrm;

static void sig_alrm(int) {
    longjmp(env_alrm, 1);
}

int main() {
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");
    if (setjmp(env_alrm) != 0)
        err_quit("read timeout");

    alarm(2);
    char line[MAXLINE];
    ssize_t n = read(STDIN_FILENO, line, MAXLINE);
    if (n < 0)
        err_sys("read error");
    alarm(0);

    write(STDOUT_FILENO, line, n);
    return 0;
}
/* 实验结果
$ time ./read_timeout_jmp 
read timeout

real	0m2.004s
user	0m0.000s
sys	0m0.000s
*/
/** 结果分析
 * 由于Linux的signal默认被中断的系统调用自动重启动，因此需要在信号处理器中
 * 手动地跳转到读取超时的状态，即在read调用之前保存环境信息，收到信号时跳转
 * 到该位置，被重启的read调用随着栈帧的回滚而终止。
 * 然而无法取得正确的errno信息，所以是err_quit而非err_sys。
 */
