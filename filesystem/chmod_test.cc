#include "../include/apue.h"
#include <sys/stat.h>

int main() {
    struct stat buf;

    // 打开set-group-ID位，关闭组用户的执行权限
    if (stat("foo", &buf) == -1)
        err_sys("stat error for foo");
    if (chmod("foo", (buf.st_mode & ~S_IXGRP) | S_ISGID) == -1)
        err_sys("chmod error for foo");

    // 设置文件绝对权限为"rw-r--r--"
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
        err_sys("chmod error for bar");
    system("ls -l foo bar");
    return 0;
}
/* 实验结果
$ ./umask_test 
-rw-r--r-- 1 xyz xyz 0 Mar 18 09:02 bar
-rw-rwSrw- 1 xyz xyz 0 Mar 18 09:02 foo
$ ./chmod_test 
-rw-r--r-- 1 xyz xyz 0 Mar 18 09:02 bar
-rw-rwSrw- 1 xyz xyz 0 Mar 18 09:02 foo
*/
