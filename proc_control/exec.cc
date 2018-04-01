#include "../include/apue.h"

const char* env_init[] = { "USER=unknown", "PATH=/tmp", nullptr };

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (execle("/home/xyz/echoall", "echoall", "myarg1",
                   "MYARG2", nullptr, env_init) < 0)
            err_sys("execle error");
        printf("cannot reach here\n");
    }
    return 0;
}
// 注意: argv和envp结尾必须是(char*)0，如果是0，可能因为char*和int长度不一致而出错
