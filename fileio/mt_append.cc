#include "../include/apue.h"
#include <thread>
#include <vector>

// 写入数字num和换行符到文件filename中
void thread_func(const char* filename, int num) {
    int fd = open(filename, O_WRONLY);
    std::string s = std::to_string(num);
    const int w = 4;
    s.resize(w);

    if (pwrite(fd, s.c_str(), w, num * w) != w)
        err_ret("pwrite error");

    off_t offset = lseek(fd, 0, SEEK_CUR);
    printf("[%d] write %s, offset = %ld\n", fd, s.c_str(), offset);
}

int main() {
    // 创建提供多线程读写的文件
    const char* filename = "text";
    int fd = creat(filename, FILE_MODE);
    close(fd);

    int n = std::thread::hardware_concurrency();
    printf("创建%d个线程同时读写数据...\n", n);
    std::vector<std::thread> threads(n);
    for (int i = 0; i < n; ++i) {
        threads[i] = std::thread(thread_func, filename, i);
    }
    for (auto& th : threads)
        th.join();
    return 0;
}
/*实验结果
$ g++ mt_append.cc -std=c++11 -pthread
$ ./a.out 
创建4个线程同时读写数据...
[6] write 3, offset = 0
[5] write 2, offset = 0
[4] write 1, offset = 0
[3] write 0, offset = 0
$ od -c text 
0000000   0  \0  \0  \0   1  \0  \0  \0   2  \0  \0  \0   3  \0  \0  \0
0000020
*/
/*结果分析
这里相当于写入了0,1,2,3这4个数到指定偏移量(分别为0,4,8,12)从而避免了重叠
但是每次pwrite写入后不会改变偏移量，offset均为0
*/
