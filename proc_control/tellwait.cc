#include "../include/apue.h"

static void charactatime(const char*);
#define TELL_WAIT()
#define WAIT_PARENT()
#define TELL_CHILD(pid)

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
/* 实验结果
$ ./tellwait 
output from parent
output from child
$ ./tellwait 
output from parent
output from child
$ ./tellwait 
output from parent
output from child
$ ./tellwait 
output from parent
output from child
$ ./tellwait 
outpouutt pfurto mf rpoamr ecnhti
ld
$ ./tellwait 
output from parent
output from child
$ ./tellwait 
output from parento
utput from child
$ ./tellwait 
output froomu tppaurte nftr
om child
$ ./tellwait 
outpouutt pfurto mf rpoamr ecnhti
ld
*/
/** 结果分析
 * 竞争条件，父进程和子进程输出发生了混合
 */
