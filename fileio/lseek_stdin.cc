#include "../include/apue.h"
//#include "apue.h"

int main() {
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        printf("cannot seek!\n");
    else
        printf("seek OK!\n");
    return 0;
}
/* 运行结果
$ ./lseek_stdin
cannot seek!
$ cat < /etc/passwd | ./a.out 
cannot seek!
$ ./a.out < /etc/passwd
seek OK!
*/
/** 说明
 * < /etc/passwd 将标准输入流重定向为文件流，所以可以lseek
 * 而cat < /etc/passwd | 则通过管道又将其重定向为标准输入流，所以不能lseek
 */
