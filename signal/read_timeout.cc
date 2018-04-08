#include "../include/apue.h"

constexpr int MAXLINE = 1024;
static void sig_alrm(int) {
    // nothing to do
}

int main() {
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

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
$ time ./read_timeout 
^C

real	0m3.855s
user	0m0.000s
sys	0m0.000s
*/
/** 结果分析
 * Linux下被signal中断的系统调用会自动重启动，因此read会一直等待。
 * 另一方面，若不自动重启，alarm和read之间会有竞争条件，若内核在两者之间阻塞过久，
 * 间隔时间超过了alarm设置的2秒，那么read仍然会一直等待而不是被alarm中断。
 */
