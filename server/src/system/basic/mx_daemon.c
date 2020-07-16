#include "uchat_server.h"



void mx_daemon(void) {
    pid_t process_id = 0;
    pid_t sid = 0;

    process_id = fork();
    if (process_id < 0) {
        fprintf("fork failed!\n");
        exit(1);
    }
    if (process_id > 0) {
        printf("process_id of child process %d \n", process_id);
        exit(0);
    }
    umask(0);
    sid = setsid();
    if (sid < 0)
        exit(1);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    if (open("/dev/null",O_RDONLY) == -1) {
        perror("open");
        exit(1);
    }
    if (open("/dev/null",O_WRONLY) == -1) {
        perror("open");
        exit(1);
    }
    if (open("/dev/null",O_RDWR) == -1) {
        perror("open");
        exit(1);
    }

}
