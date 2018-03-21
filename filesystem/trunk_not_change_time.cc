#include "../include/apue.h"
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    struct stat statbuf;
    struct timespec times[2];
    for (int i = 1; i < argc; ++i) {
        if (stat(argv[i], &statbuf) == -1) {  // 备份文件访问时间和修改时间
            err_ret("%s: stat error", argv[i]);
            continue;
        }
        int fd = open(argv[i], O_RDWR | O_TRUNC);  // 截断文件数据
        if (fd == -1) {
            err_ret("%s: open error", argv[i]);
            continue;
        }
        // 恢复文件访问时间和修改时间
        times[0] = statbuf.st_atim;
        times[1] = statbuf.st_mtim;
        if (futimens(fd, times) == -1)
            err_ret("%s: futimes error", argv[i]);
        close(fd);
    }
    return 0;
}
/* 实验结果
$ ls -l foo bar
-rw-rw-r-- 1 xyz xyz 5 Mar 18 11:24 bar
-rw-rw-r-- 1 xyz xyz 5 Mar 18 11:24 foo
$ ls -lu foo bar
-rw-rw-r-- 1 xyz xyz 5 Mar 18 11:23 bar
-rw-rw-r-- 1 xyz xyz 5 Mar 18 11:23 foo
$ date
Sun Mar 18 11:26:46 PDT 2018
$ ./trunk_not_change_time foo bar
$ ls -l foo bar
-rw-rw-r-- 1 xyz xyz 0 Mar 18 11:24 bar
-rw-rw-r-- 1 xyz xyz 0 Mar 18 11:24 foo
$ ls -lu foo bar
-rw-rw-r-- 1 xyz xyz 0 Mar 18 11:23 bar
-rw-rw-r-- 1 xyz xyz 0 Mar 18 11:23 foo
$ ls -lc foo bar
-rw-rw-r-- 1 xyz xyz 0 Mar 18 11:26 bar
-rw-rw-r-- 1 xyz xyz 0 Mar 18 11:26 foo
*/
/** 结果分析
 * ls的-u选项显示最后访问时间，-l选项显示最后修改时间，-c选项显示状态更改时间
 * 结果显示ls -l和ls -lu的结果未变，而ls -lc则显示为当前时间，文件内容则被清空。
 */
