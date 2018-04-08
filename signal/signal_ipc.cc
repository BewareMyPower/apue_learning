#include "../include/apue.h"

static volatile sig_atomic_t sigflag = 0;  // 由信号处理器设为非零值
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int) {  // SIGUSR1和SIGUSR2的处理器
    sigflag = 1;
}

void TELL_WAIT(void) {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR2) error");

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    // 阻塞SIGUSR1和SIGUSR2，保存当前信号掩码以便之后恢复
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1)
        err_sys("SIG_BLOCK error");
}

void TELL_CHILD(pid_t pid) {
    kill(pid, SIGUSR1);
}

void WAIT_CHILD(void) {
    while (sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) == -1)
        err_sys("SIG_SETMASK error");
}

void TELL_PARENT(pid_t pid) {
    kill(pid, SIGUSR2);
}

void WAIT_PARENT(void) {
    while (sigflag == 0)
        sigsuspend(&zeromask);  // 等待父进程发送信号
    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) == -1)
        err_sys("SIG_SETMASK error");
}

static void charactatime(const char*);

int main() {
    TELL_WAIT();

    pid_t pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        WAIT_PARENT();    // 等待父进程先执行
        charactatime("output from child\n");
    } else {
        charactatime("output from parent\n");
        TELL_CHILD(pid);  // 父进程执行完毕，通知子进程
    }
    return 0;
}

static void charactatime(const char* str) {
    setbuf(stdout, NULL);  // 设置为无缓冲，1次打印1个字符
    for (auto p = str; *p != '\0'; ++p)
        putc(*p, stdout);
}
