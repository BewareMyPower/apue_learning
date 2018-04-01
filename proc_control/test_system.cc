#include "../include/apue.h"
#include "system.h"

int main() {
    int status;

    if ((status = system("date")) == -1)
        err_sys("system() error");
    
    pr_exit(status);

    if ((status = system("nosuchcommand")) == -1)
        err_sys("system() error");

    pr_exit(status);

    if ((status = system("who; exit 44")) == -1)
        err_sys("system() error");

    pr_exit(status);

    return 0;
}
