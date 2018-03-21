#include "../../include/apue.h"

int main() {
    int fd = open("tempfile", O_RDWR | O_CREAT | O_APPEND, FILE_MODE);
    write(fd, "hello", 5);
    write(fd, "world", 5);
    if (lseek(fd, 0, SEEK_SET) < 0)
        err_sys("lseek error");
    write(fd, "newmsg", 6);
    return 0;
}
/*输出结果
文件tempfile的内容为helloworldnewmsg
说明设置O_APPEND模式后lseek无用，因为每次调用write时
会自动定位到流的首部
*/
