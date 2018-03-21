#include "../include/apue.h"

int main(int argc, char* argv[]) {
    if (argc != 2)
        err_quit("usage: %s <pathname>", argv[0]);
    if (access(argv[1], R_OK) == -1)
        err_ret("access error for %s", argv[1]);
    else
        printf("read access OK\n");
    if (open(argv[1], O_RDONLY) == -1)
        err_ret("open error for %s", argv[1]);
    else
        printf("open for reading OK\n");
    return 0;
}
/* 实验结果
$ ./access_test access_test
read access OK
open for reading OK
$ ls -l access_test
-rwxrwxr-x 1 xyz xyz 22888 Mar 18 08:07 access_test
$ ./access_test access_test
read access OK
open for reading OK
$ ls -l /etc/shadow
-rw-r----- 1 root shadow 1425 Mar 18 07:35 /etc/shadow
$ ./access_test /etc/shadow
access error for /etc/shadow: Permission denied
open error for /etc/shadow: Permission denied
$ su
Password:
# chown root access_test
# chmod u+s access_test
# ls -l access_test
-rwsrwxr-x 1 root xyz 22888 Mar 18 08:07 access_test
# exit
exit
$ ./access_test /etc/shadow
access error for /etc/shadow: Permission denied
open for reading OK
*/
/** 结果分析
 * /etc/shadow文件只提供root用户和组的读取权限，因此open会失败
 * 这里把access_test文件的用户改为root，并设置set-user-id位，
 * 此时执行access_test的进程有效用户ID为root的ID，因此可以只读模式打开/etc/shadow，
 * 但是access是以实际用户ID检查，因此仍然会汇报错误
 */
