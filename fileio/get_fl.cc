#include "../include/apue.h"

int main(int argc, char* argv[]) {
    if (argc != 2)
        err_quit("usage: %s <descriptor#>", argv[0]);
    
    int fd = std::stoi(argv[1]);
    int val = fcntl(fd, F_GETFL, 0);
    if (val < 0)
        err_sys("fcntl error for fd %d", fd);

    switch (val & O_ACCMODE) {
    case O_RDONLY:
        printf("read only");
        break;
    case O_WRONLY:
        printf("write only");
        break;
    case O_RDWR:
        printf("read write");
        break;
    default:
        err_dump("unknown access mode");
    }

    if (val & O_APPEND)
        printf(", append");
    if (val & O_NONBLOCK)
        printf(", nonblocking");
    if (val & O_SYNC)
        printf(", synchronous writes");

    if (val & O_FSYNC)
        printf(", synchronous writes");

    putchar('\n');
    return 0;
}
/*运行结果
$ ./get_fl < /dev/tty
usage: ./get_fl <descriptor#>
$ ./get_fl 0 < /dev/tty
read only
$ ./get_fl 1 > temp.foo
$ cat temp.foo 
write only
$ ./get_fl 2 2>>temp.foo
write only, append
$ cat temp.foo
write only
$ ./get_fl 5 5<>temp.foo
read write
$ ./get_fl 0 >temp.foo
$ cat temp.foo
read write, append
*/
/*说明
对于< /dev/tty，相当于将标准输入流重定向为打开的/dev/tty文件
因此文件描述符0对应的是open打开的/dev/tty文件，但是重定向并不
改变文件状态标志，所以结果仍然是标准输入流的文件状态标志。
同理，为了能获得标准输出流和标准错误流的文件状态标志，也需要重定向。

<file等价于0<file
>file等价于1>file

*/
