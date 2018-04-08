#include "../include/apue.h"
#include <pwd.h>

static void my_alarm(int) {
    printf("in signal handler\n");
    struct passwd* rootptr = getpwnam("root");
    if (!rootptr)
        err_sys("getpwnam(root) error");
    alarm(1);  // 会发送SIGALRM信号，再次调用my_alarm处理
    // 由于使用signal注册信号处理器的行为是不可预期的(这里是屏蔽该信号)
}

int main() {
    struct passwd* ptr;

    signal(SIGALRM, my_alarm);
    alarm(1);
    for (;;) {
        if ((ptr = getpwnam("xyz")) == NULL)
            err_sys("getpwnam error");
        if (strcmp(ptr->pw_name, "xyz") != 0)
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
    }
    return 0;
}
