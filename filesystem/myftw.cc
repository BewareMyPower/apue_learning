#include "../include/apue.h"
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <functional>

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

inline void show_file_cnt(long nfile, const char* description) {
    printf("%-15s = %7ld, %5.2lf %%\n", description, nfile, nfile * 100.0 / ntot);
}

class PathBuffer {
public:
    PathBuffer() = default;
    PathBuffer(const char* s) { add_file(s); }
    PathBuffer(const PathBuffer&) = delete;

    bool add_file(const char* name);  // 添加文件名到路径末尾，若添加失败则返回false
    void resize(size_t newlen);  // 改变路径大小为newlen
    bool is_dir() const;
    const char* fullname() const { return _str; }
    const char* basename() const { return _basestr + 1; }
    size_t length() const { return _len; }

    mutable struct stat _stat;  // 路径对应的元数据
    bool lstat() const;  // 获取路径对应文件的元数据，若无法获取则返回false
private:
    char _str[PATH_MAX] = {0};  // 实际路径字符串缓冲区
    const char* _basestr = _str;  // 指向文件名
    size_t _len = 0;  // 路径长度
};

void deal_with_path(const PathBuffer& path, int) {
    path.lstat();
    switch (path._stat.st_mode & S_IFMT) {
        case S_IFREG:    nreg++;   break;
        case S_IFBLK:    nblk++;   break;
        case S_IFCHR:    nchr++;   break;
        case S_IFIFO:    nfifo++;  break;
        case S_IFLNK:    nslink++; break;
        case S_IFSOCK:   nsock++;  break;
        case S_IFDIR:    ndir++;   break;
        default: err_msg("%s: unknown type", path.fullname());
    }
//    if (depth > 0) {
//        printf("%*s\\--", depth * 3, " ");
//    }
//    printf("%s\n", path.basename());
}


class FileTreeIterator {
public:
    using CallBack = std::function<void (const PathBuffer&, int)>;
    FileTreeIterator(const char* dirname, CallBack func);

    void dfs(int depth) {
        DIR* dp = opendir(_path.fullname());
        if (!dp) {
            err_ret("opendir %s failed", _path.fullname());
            return;
        }
        struct dirent* dirp = nullptr;
        while ((dirp = readdir(dp)) != nullptr) {
            if (strcmp(dirp->d_name, ".") == 0 ||
                strcmp(dirp->d_name, "..") == 0)
                continue;
            size_t oldlen = _path.length();
            // 递归遍历子目录
            _path.add_file(dirp->d_name);
            _func(_path, depth);
            if (_path.is_dir()) 
                dfs(depth + 1);
            // 回到递归前的状态
            _path.resize(oldlen);
        }
    
        closedir(dp);
    }

private:
    PathBuffer _path;
    std::function<void (const PathBuffer&, int)> _func;
};

int main(int argc, char* argv[]) {
    if (argc != 2)
        err_quit("usage: %s <starting-pathname>", argv[0]);
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1;  // 避免和0整除
    FileTreeIterator tree(argv[1], deal_with_path);
    tree.dfs(0);
    show_file_cnt(nreg, "regular files");
    show_file_cnt(ndir, "directories");
    show_file_cnt(nblk, "block special");
    show_file_cnt(nchr, "char special");
    show_file_cnt(nfifo, "FIFOs");
    show_file_cnt(nslink, "symbol links");
    show_file_cnt(nsock, "sockets");
    return 0;
}

inline bool PathBuffer::lstat() const {
    if (::lstat(_str, &_stat) == -1) {
        err_ret("lstat %s failed", _str);
        return false;
    }
    return true;
}

inline bool PathBuffer::add_file(const char* name) {
    if (!name)
        return false;
    if (_str[_len] != '/' && name[0] != '/')
        _str[_len++] = '/';
    size_t namelen = strlen(name);
    if (namelen + _len + 1 < PATH_MAX) {
        strncpy(_str + _len, name, namelen);
        _len += namelen;
        _str[_len] = '\0';
        for (_basestr = _str + _len; *_basestr != '/'; --_basestr) {}
        return true;
    } else {
        err_msg("Path is too long!\n  (%s%s)\n", _str, name);
        return false;
    }
}

inline void PathBuffer::resize(size_t newlen) {
    newlen = (newlen >= PATH_MAX) ? (PATH_MAX - 1) : newlen;
    _len = newlen;
    _str[_len] = '\0';
    for (_basestr = _str + _len; _basestr != _str && *_basestr != '/'; --_basestr) { }
}

bool PathBuffer::is_dir() const {
    if (!lstat())
        return false;
    return S_ISDIR(_stat.st_mode);
}

FileTreeIterator::FileTreeIterator(const char* dirname, CallBack func) : _func(func){
    if (!_path.add_file(dirname))
        exit(1);
}
