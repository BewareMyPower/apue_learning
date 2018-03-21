#include "../include/apue.h"
#include <sys/stat.h>

const char* get_file_type(const struct stat& buf) {
    if (S_ISREG(buf.st_mode))
        return "regular";
    else if (S_ISDIR(buf.st_mode))
        return "directory";
    else if (S_ISCHR(buf.st_mode))
        return "character special";
    else if (S_ISBLK(buf.st_mode))
        return "block special";
    else if (S_ISFIFO(buf.st_mode))
        return "fifo";
    else if (S_ISLNK(buf.st_mode))
        return "symbolic link";
    else if (S_ISSOCK(buf.st_mode))
        return "socket";
    else
        return "** unknown type **";
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        struct stat buf;
        printf("%s: ", argv[i]);
        if (lstat(argv[i], &buf) == -1) {
            err_ret("lstat error");
            continue;
        }
        printf("%s\n", get_file_type(buf));
    }
    return 0;
}
/* 实验结果
~/apue_learning/filesystem$ ./filetype /etc/passwd /etc/ /dev/log /dev/tty \
> /var/lib/oprofile/opd_pipe /dev/sr0 /dev/cdrom 
/etc/passwd: regular
/etc/: directory
/dev/log: symbolic link
/dev/tty: character special
lstat error: No such file or directory
/var/lib/oprofile/opd_pipe: /dev/sr0: block special
/dev/cdrom: symbolic link
*/
