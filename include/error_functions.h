#ifndef _ERROR_FUNCTIONS_H_
#define _ERROR_FUNCTIONS_H_

#include "format_string.h"

/**
 * 非致命性错误，系统调用无关
 * 打印消息后返回
 */
template <typename ...Args>
inline void err_msg(const char* fmt, Args... args) {
    try {
        throw_error(fmt, std::forward<Args>(args)...);
    } catch (std::runtime_error& e) {
        fprintf(stderr, "%s\n", e.what());
    }
}

/**
 * 非致命性错误，系统调用相关
 * 打印消息后返回
 */
template <typename ...Args>
inline void err_ret(const char* fmt, Args... args) {
    try {
        throw_syserror(fmt, std::forward<Args>(args)...);
    } catch (std::runtime_error& e) {
        fprintf(stderr, "%s\n", e.what());
    }
}

/**
 * 非致命性错误，系统调用无关
 * 显式传递错误码
 * 打印消息后返回
 */
template <typename ...Args>
inline void err_cont(int error, const char* fmt, Args... args) {
    try {
        throw_syserror(error, fmt, std::forward<Args>(args)...);
    } catch (std::runtime_error& e) {
        fprintf(stderr, "%s\n", e.what());
    }
}

/**
 * 致命性错误，系统调用无关
 * 打印消息后终止进程
 */
template <typename ...Args>
inline void err_quit(const char* fmt, Args... args) {
    err_msg(fmt, std::forward<Args>(args)...);
    exit(1);
}

/**
 * 致命性错误，系统调用相关
 * 打印消息后终止进程
 */
template <typename ...Args>
inline void err_sys(const char* fmt, Args... args) {
    err_ret(fmt, std::forward<Args>(args)...);
    exit(1);
}

/**
 * 非致命性错误，系统调用无关
 * 显式传递错误码
 * 打印消息后终止进程
 */
template <typename ...Args>
inline void err_exit(int error, const char* fmt, Args... args) {
    err_cont(error, fmt, std::forward<Args>(args)...);
    exit(1);
}

/**
 * 致命性错误，系统调用相关
 * 打印消息，转储核心后后终止进程
 */
template <typename ...Args>
inline void err_dump(const char* fmt, Args... args) {
    err_ret(fmt, std::forward<Args>(args)...);
    abort();
}

#endif
