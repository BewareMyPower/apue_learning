#ifndef _APUE_H_
#define _APUE_H_

// 通用常量的定义
#include "const_vars.h"

// 字符串格式化方法
#include "format_string.h"

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <algorithm>

// 错误处理库
#include "error_functions.h"

// 文件标志位操作方法
#include "flag_op.h"

// 系统调用的包裹函数
#include "func_wrapper.h"

// 打印终止状态的描述信息
#include "pr_exit.h"

#endif
