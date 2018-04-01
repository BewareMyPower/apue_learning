// system.cc: system函数的自定义实现，对信号没有处理
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int my_system(const char* cmdstring) {  // 忽略了信号处理
    if (cmdstring == NULL) 
        return 1;  // 空命令永远正确

    int status;
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {  // 子进程
        // "-c"选项告诉shell程序取下一个命令行参数作为命令输入
        execl("/bin/bash", "sh", "-c", cmdstring, (char*)NULL);
        _exit(127);
    } else {  // 父进程等待exec进程执行完毕，并忽略中断信号
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
    }

    return status;
}
/* 实验结果
$ make test_system 
g++ -std=c++11 -Wall -I../include -D_GNU_SOURCE  system.cc -c
g++ -std=c++11 -Wall -I../include -D_GNU_SOURCE  test_system.cc system.o ../lib/libutil.a -o test_system
$ ./test_system 
Sun Apr  1 09:13:38 PDT 2018
normal termination, exit status = 0
sh: nosuchcommand: command not found
normal termination, exit status = 127
xyz      tty7         2018-03-31 23:33 (:0)
normal termination, exit status = 44
*/
