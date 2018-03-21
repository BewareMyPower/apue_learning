#include "../include/apue.h"
#include <sys/stat.h>

constexpr int RWRWRW = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

int main() {
    umask(0);
    if (creat("foo", RWRWRW) == -1)
        err_sys("creat error for foo");
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (creat("bar", RWRWRW) == -1)
        err_sys("creat error for bar");
    system("ls -l foo bar");
    return 0;
}
/* 实验结果
$ umask 
0002
$ ./umask_test 
-rw------- 1 xyz xyz 0 Mar 18 08:30 bar
-rw-rw-rw- 1 xyz xyz 0 Mar 18 08:30 foo
$ rm foo bar
$ touch foo
$ ls -l foo
-rw-rw-r-- 1 xyz xyz 0 Mar 18 08:42 foo
$ mkdir bar
$ ls -l | grep "bar"
drwxrwxr-x 2 xyz xyz  4096 Mar 18 08:43 bar
*/
/** 结果分析
 * umask命令取得当前终端的默认屏蔽权限，002即屏蔽其他用户的写入权限
 * 普通文件的默认权限是rw-rw-rw-(666)，目录文件的默认权限是rwxrwxrwx(777)
 * 因此默认屏蔽下普通文件的权限是rw-rw-r--，目录文件的权限是rwxwrwxr-x
 * 这里首先用umask(0)取消对所有权限位的屏蔽，因此创建的foo文件权限为rw-rw-rw-
 * 之后umask设置了对组用户/其他用户的读写权限的屏蔽，因此创建的bar文件权限为rw-------
 */
