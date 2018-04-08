#include "../include/apue.h"
#include <signal.h>

static void sig_usr(int);

int main() {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("can't catch SIGUSR1");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("can't catch SIGUSR2");
    for ( ; ; )
        pause();
    return 0;
}

static void sig_usr(int signo) {
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGUSR2)
        printf("received SIGUSR2\n");
    else
        err_dump("received signal %d\n", signo);
}
/* 实验结果
$ ./signal_example &
[1] 5243
$ kill -USR1 %1
received SIGUSR1
$ kill -USR2 %1
received SIGUSR2
$ kill -TERM
kill: usage: kill [-s sigspec | -n signum | -sigspec] pid | jobspec ... or kill -l [sigspec]
$ kill %1
$ 
[1]+  Terminated              ./signal_example
*/
