#include "../include/apue.h"
//#include "apue.h"

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main() {
    int fd = creat("file.hole", FILE_MODE);
    if (fd < 0)
        err_sys("create error");

    if (write(fd, buf1, 10) != 10)
        err_sys("buf1 write error");
    // offset now = 10
    
    if (lseek(fd, 16384, SEEK_SET) == -1)
        err_sys("lseek error");
    // offset now = 16384

    if (write(fd, buf2, 10) != 10)
        err_sys("buf2 write error");
    // offset now = 16394
    
    // 创建和上述文件同样大小的非空洞文件
    int fd2 = creat("file.nohole", FILE_MODE);
    if (fd2 < 0)
        err_sys("create error");

    constexpr int LARGE_BUFSIZ = 16394;
    char large_buf[LARGE_BUFSIZ];
    bzero(large_buf, LARGE_BUFSIZ);

    if (write(fd2, large_buf, LARGE_BUFSIZ) != LARGE_BUFSIZ)
        err_sys("large_buf write error");

    return 0;
}
/* 运行结果
$ ./hole_file
$ ls | grep -E "^file.*" | xargs ls -ls
 8 -rw-r--r-- 1 xyz xyz 16394 Mar  2 09:13 file.hole
20 -rw-r--r-- 1 xyz xyz 16394 Mar  2 09:13 file.nohole
$ od -c file.hole 
0000000   a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
0040000   A   B   C   D   E   F   G   H   I   J
0040012

*/
/** 说明
 * 可以发现ls显示的文件大小是16394，但是od(1)查看每个字节时中间部分全部是'\0'
 * ls的-s参数显示了占用的磁盘块，带空洞的文件只占用了8个磁盘块，而同样大小的
 * 不带空洞的文件占用了20个磁盘块。
 */
