#include "../include/apue.h"

volatile sig_atomic_t quitflag = 0;  // 由信号处理器设置为非零值

static void sig_int(int signo) {  // 对SIGINT和SIGQUIT的处理器
    if (signo == SIGINT)
        printf("\ninterrupt\n");  // 注意这里printf非异步信号安全
    else if (signo == SIGQUIT)
        quitflag = 1;  // 设置为1终止主循环
}

int main() {
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    if (signal(SIGQUIT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");

    sigset_t newmask, oldmask, zeromask;
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    // 阻塞SIGQUIT并保存当前信号掩码
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1)
        err_sys("SIG_BLOCK error");

    while (quitflag == 0)
        sigsuspend(&zeromask);  // 暂时取消对信号的阻塞，直到由信号改变quitflag

    printf("收到SIGQUIT信号!\n");

    quitflag = 0;

    // 由于sigsuspend在收到信号后会把信号掩码重置为之前的值
    // 因此此时SIGQUIT仍然被阻塞，这里重置信号掩码，取消对SIGQUIT的阻塞
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");

    return 0;
}
