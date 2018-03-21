// func_wrapper.cc: 系统调用的C++包裹函数的实现
#include "../include/flag_op.h"
#include "../include/error_functions.h"
#include <fcntl.h>
#include <unistd.h>

int Open(const char* filename, int flags, mode_t mode) {
    int fd = open(filename, flags, mode);
    if (fd == -1)
        err_sys("open %s failed", filename);
    return fd;
}

ssize_t Write(int fd, const void* buf, size_t cnt) {
    ssize_t n = write(fd, buf, cnt);
    if (n == -1)
        err_sys("%d write error", fd);
    return n;
}

ssize_t Read(int fd, void* buf, size_t cnt) {
    ssize_t n = read(fd, buf, cnt);
    if (n == -1)
        err_sys("%d read error", fd);
    return n;
}
