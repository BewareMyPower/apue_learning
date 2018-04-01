// tsys.cc: 将命令行参数1作为shell命令执行，并打印终止状态
#include "../include/apue.h"

int main(int argc, char* argv[]) {
    if (argc < 2)
        err_quit("command-line argument required");

    int status;
    if ((status = system(argv[1])) < 0)
        err_sys("system() error");

    pr_exit(status);
    return 0;
}
