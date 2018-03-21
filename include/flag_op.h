// flag_operation.h: 文件标志的操作函数
#ifndef _FLAG_OP_H_
#define _FLAG_OP_H_

/**
 * 功能: 打开文件对应状态标志
 * 参数:
 *   fd    文件描述符
 *   flags 需要打开的标志
 * 返回:
 *   无返回值，若设置错误则打印错误消息后终止进程
 */
void set_fl(int fd, int flags); 

/**
 * 功能: 清除文件对应状态标志
 * 参数:
 *   fd    文件描述符
 *   flags 需要打开的标志
 * 返回:
 *   无返回值，若设置错误则打印错误消息后终止进程
 */
void clr_fl(int fd, int flags); 

#endif
