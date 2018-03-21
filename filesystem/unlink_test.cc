#include "../include/apue.h"

int main() {
    if (open("tempfile", O_RDWR) == -1)
        err_sys("open error");
    if (unlink("tempfile") == -1)
        err_sys("unlink error");
    printf("file unlinked\n");
    sleep(15);
    printf("done\n");
    return 0;
}
/* 实验结果
$ cp unlink_test.cc tempfile
$ ls -l tempfile 
-rw-rw-r-- 1 xyz xyz 284 Mar 18 10:06 tempfile
$HOME
Filesystem     1K-blocks    Used Available Use% Mounted on
/dev/sda1       23607740 7945556  14439940  36% /
$ ./unlink_test &
[1] 12893
$ file unlinked
ls -l tempfile 
ls: cannot access 'tempfile': No such file or directory
$HOME
Filesystem     1K-blocks    Used Available Use% Mounted on
/dev/sda1       23607740 7945556  14439940  36% /
$ done
$HOME
Filesystem     1K-blocks    Used Available Use% Mounted on
/dev/sda1       23607740 7945552  14439944  36% /
[1]+  Done                    ./unlink_test
*/
/** 结果分析
 * 可以发现文件被unlink后直到调用unlink的进程终止时才会删除文件的内容
 */
