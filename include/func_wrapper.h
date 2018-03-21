// func_wrapper.h: 系统调用的C++包裹函数声明
// 包裹函数的名称为对应系统调用的函数名的首字母大写，比如Open对应open
#ifndef _FUNC_WRAPPER_H_
#define _FUNC_WRAPPER_H_
#include <sys/types.h>
#include "../include/const_vars.h"

int Open(const char* filename, int flags, mode_t mode = FILE_MODE);

ssize_t Write(int fd, const void* buf, size_t cnt);

ssize_t Read(int fd, void* buf, size_t cnt);

#endif
