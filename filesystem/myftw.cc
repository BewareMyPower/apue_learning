#include "../include/apue.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string>

// 该降序遍历文件层次结构，统计各类文件的百分比
static void myftw(std::string& pathname);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

inline void show_file_cnt(long nfile, const char* description) {
    printf("%-15s = %7ld, %5.2lf %%\n", description, nfile, nfile * 100.0 / ntot);
}

enum ftw_state {
    F,    // 目录以外的文件
    D,    // 目录
    DNR,  // 无法读取的目录
    NS    // 无法调用stat的文件
};

int main(int argc, char* argv[]) {
    if (argc != 2)
        err_quit("usage: myftw <starting-pathname>");
    std::string pathname = argv[1];
    myftw(pathname);

    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1;  // 避免和0整除
    show_file_cnt(nreg, "regular files");
    show_file_cnt(ndir, "directories");
    show_file_cnt(nblk, "block special");
    show_file_cnt(nchr, "char special");
    show_file_cnt(nfifo, "FIFOs");
    show_file_cnt(nslink, "symbol links");
    show_file_cnt(nsock, "sockets");
    return 0;
}

// 获取文件类型，若无法调用lstat函数则返回-1
inline mode_t get_file_mode(const std::string& pathname) {
    struct stat statbuf;
    if (lstat(pathname.c_str(), &statbuf) == -1)
        return -1;
    return statbuf.st_mode;
}

// 统计文件数量
inline void count_file(const std::string& pathname, ftw_state state, int mode = -1) {
    switch (state) {
        case ftw_state::F:
            switch (mode & S_IFMT) {
                case S_IFREG:    nreg++;   break;
                case S_IFBLK:    nblk++;   break;
                case S_IFCHR:    nchr++;   break;
                case S_IFIFO:    nfifo++;  break;
                case S_IFLNK:    nslink++; break;
                case S_IFSOCK:   nsock++;  break;
                case S_IFDIR: err_dump("for S_IFDIR for %s", pathname.c_str());
            }
            break;
        case ftw_state::D:
            ndir++;
            break;
        case ftw_state::DNR:
            err_ret("can't read directory %s", pathname.c_str());
            break;
        case ftw_state::NS:
            err_ret("stat error for %s", pathname.c_str());
            break;
        default:
            err_dump("unknown type %d for pathname %s", mode, pathname.c_str());
    }
}

static void myftw(std::string& pathname) {
    DIR* dp = opendir(pathname.c_str());
    if (!dp) {
        count_file(pathname, ftw_state::DNR);
        return;
    }

    struct dirent* dirp = nullptr;
    if (pathname.back() != '/')  // pathname不可能为空，否则之前opendir就会出错
        pathname.append("/");
    size_t oldlen = pathname.size();

    while ((dirp = readdir(dp)) != nullptr) {
        if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
            continue;
        pathname.append(dirp->d_name);

        // 判断文件类型，若为目录则递归处理
        mode_t mode = get_file_mode(pathname);
        if (mode != (mode_t)-1) {
            if (S_ISDIR(mode)) {
                count_file(pathname, ftw_state::D, mode);
                myftw(pathname);
            } else {
                count_file(pathname, ftw_state::F, mode);
            }
        } else {
            count_file(pathname, ftw_state::NS);
        }

        pathname.resize(oldlen);
    }

    if (closedir(dp) == -1)
        err_ret("can't close directory %s", pathname.c_str());
}

/* 实验结果
$ time ./myftw /dev/
regular files   =       8,  1.69 %
directories     =      26,  5.49 %
block special   =      13,  2.74 %
char special    =     206, 43.46 %
FIFOs           =       0,  0.00 %
symbol links    =     221, 46.62 %
sockets         =       0,  0.00 %

real	0m0.003s
user	0m0.000s
sys	0m0.000s
$ time ./myftw /usr/
regular files   =  213476, 65.74 %
directories     =   44606, 13.74 %
block special   =       0,  0.00 %
char special    =       0,  0.00 %
FIFOs           =       0,  0.00 %
symbol links    =   66624, 20.52 %
sockets         =       0,  0.00 %

real	0m3.129s
user	0m0.012s
sys	0m1.552s
*/
