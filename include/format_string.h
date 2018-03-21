#ifndef _FORMAT_STRING_H_
#define _FORMAT_STRING_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <vector>
#include <stdexcept>

/**
 * 功能: 用类似printf的方式构建C++标准字符串
 * 参数: 同printf(3)的输入参数
 * 返回: 调用printf(format, args...)时打印到屏幕上的字符串
 */
template <typename ...Args>
inline std::string format_string(const char* format, Args... args) {
    constexpr size_t oldlen = BUFSIZ;
    char buffer[oldlen];  // 默认栈上的缓冲区

    size_t newlen = snprintf(&buffer[0], oldlen, format, args...);
    newlen++;  // 算上终止符'\0'

    if (newlen > oldlen) {  // 默认缓冲区不够大，从堆上分配
        std::vector<char> newbuffer(newlen);
        snprintf(newbuffer.data(), newlen, format, args...);
        return std::string(newbuffer.data());
    }

    return buffer;
}

/**
 * 功能: 抛出(format, args...)格式化的异常
 * 参数:
 *   Args 用来格式化字符串的参数列表
 *   (format, args)同printf(3)的输入参数
 * 返回: 无
 */
template <typename ...Args>
inline void throw_error(const char* format, Args... args) {
    throw std::runtime_error(format_string(format, std::forward<Args>(args)...));
}

inline void throw_error(const char* msg) {
    throw std::runtime_error(msg);
}

/**
 * 功能: 抛出(format, args...)格式化的异常，附加系统错误信息
 * 参数:
 *   Args 用来格式化字符串的参数列表
 *   (format, args)同printf(3)的输入参数
 * 返回: 无
 */
template <typename ...Args>
inline void throw_syserror(const char* format, Args... args) {
    auto errmsg = format_string(format, std::forward<Args>(args)...);
    char buf[BUFSIZ];
    errmsg = errmsg + ": " + strerror_r(errno, buf, BUFSIZ);
    throw std::runtime_error(errmsg);
}

inline void throw_syserror(const char* msg) {
    char buf[BUFSIZ];
    std::string errmsg = std::string(msg) + ": "
        + strerror_r(errno, buf, BUFSIZ);
    throw std::runtime_error(errmsg);
}

/**
 * 功能: 抛出(format, args...)格式化的异常，附加系统错误信息
 * 参数:
 *   errnum 系统错误对应的错误码
 *   Args 用来格式化字符串的参数列表
 *   (format, args)同printf(3)的输入参数
 * 返回: 无
 */
template <typename ...Args>
inline void throw_syserror(int errnum, const char* format, Args... args) {
    auto errmsg = format_string(format, std::forward<Args>(args)...);
    char buf[BUFSIZ];
    errmsg = errmsg + ": " + strerror_r(errnum, buf, BUFSIZ);
    throw std::runtime_error(errmsg);
}

inline void throw_syserror(int errnum, const char* msg) {
    char buf[BUFSIZ];
    std::string errmsg = std::string(msg) + ": "
        + strerror_r(errnum, buf, BUFSIZ);
    throw std::runtime_error(errmsg);
}

#endif
