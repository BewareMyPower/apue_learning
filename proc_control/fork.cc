#include "../include/apue.h"

int g_var = 6;  // 初始化数据段
char buf[] = "a write to stdout\n";

int main() {
    int var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
        err_sys("write error");
    printf("before fork\n");

    pid_t pid = fork();
    if (pid < 0) {
        err_sys("fork");
    } else if (pid == 0) {  // 子进程
        g_var++;
        var++;
    } else {  // 父进程
        sleep(2);
    }

    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), g_var, var);
    return 0;
}
/* 实验结果
$ make fork
g++ -std=c++11 -Wall -I../include -D_GNU_SOURCE  fork.cc -o fork
$ ./fork 
a write to stdout
before fork
pid = 6533, glob = 7, var = 89
pid = 6532, glob = 6, var = 88
$ ./fork >temp.out
$ cat temp.out 
a write to stdout
before fork
pid = 6537, glob = 7, var = 89
before fork
pid = 6536, glob = 6, var = 88
*/
/** 结果分析
 * 代码功能为首先用write打印字符串，再用printf打印字符串(包括换行符)，然后创建子进程
 * 父进程sleep(2)保证子进程先修改变量，然后两个线程各自用printf打印各变量的值
 * 可以发现子进程(ID: 6537)修改的全局变量、局部变量未能影响父进程的对应变量，说明数据不共享。
 * 不同之处在于，./fork >temp.out重复打印了before fork。
 * 原因在于这里将标准输入重定向到文件写入上，printf对于标准输出流是行缓冲的，
 * 因此遇到换行符会flush，即fork之前清空了stdio的缓冲区。
 * 但是printf对于文件流是全缓冲的，因此fork之前"before fork\n"仍停留在缓冲区中，随着fork
 * 一起复制给了子进程。
 * PS: stdio对于标准错误流是无缓冲的，因此把代码中的printf改成fprintf调用后，
 * 即使用./fork 2>temp.out把标准错误流重定向到文件流中，仍然只有一个before fork输出。
 */
