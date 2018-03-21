// flag_op.cc: 文件标志的操作函数具体实现
#include "../include/flag_op.h"
#include "../include/error_functions.h"
#include <fcntl.h>

/**
 * 功能: 打开文件对应状态标志
 * 参数:
 *   fd    文件描述符
 *   flags 需要打开的标志
 * 返回:
 *   无返回值，若设置错误则打印错误消息后终止进程
 */
void set_fl(int fd, int flags) {
    int val = fcntl(fd, F_GETFL, 0);
    if (val < 0)
        err_sys("fcntl F_GETFL error");

    val |= flags;

    if (fcntl(fd, F_GETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}

/**
 * 功能: 清除文件对应状态标志
 * 参数:
 *   fd    文件描述符
 *   flags 需要打开的标志
 * 返回:
 *   无返回值，若设置错误则打印错误消息后终止进程
 */
void clr_fl(int fd, int flags) {
    int val = fcntl(fd, F_GETFL, 0);
    if (val < 0)
        err_sys("fcntl F_GETFL error");

    val &= ~flags;

    if (fcntl(fd, F_GETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}
