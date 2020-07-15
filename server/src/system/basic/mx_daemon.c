#include "uchat_server.h"

void mx_daemon(void) {
    pid_t process_id = 0;
    pid_t sid = 0;

    process_id = fork();
    if (process_id < 0) {
        mx_printerr("uchat_server: fork failed");
        exit(1);
    }
    if (process_id > 0) {
        printf("uchat_server: process_id of child process %d \n", process_id);
        printf("uchat_server: process_id of child process %d \n", process_id);
        exit(0);
    }
    umask(0);
    sid = setsid();
    if (sid < 0)
        exit(1);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
