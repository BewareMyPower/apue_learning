#pragma once

// 覆盖系统的system()函数
int my_system(const char*);
#define system(cmdstring) my_system(cmdstring)
