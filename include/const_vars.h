// const_vars.h: 一些通用常量的定义
#ifndef _CONST_VARS_H_
#define _CONST_VARS_H_
#include <fcntl.h>

// 默认文件访问权限"rw-r--r--"
constexpr int FILE_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

// 默认目录访问权限"rwxr-xr-x"
constexpr int DIR_MODE = FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH;

#endif
