#include "uchat_server.h"

void mx_logger(const char* tag, const char* message) {
    char *log;
    int fd;

    time_t now;
    time(&now);
    asprintf(&log, "%s [%s]: %s\n", ctime(&now), tag, message);
    fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        printf("Error logs\n");
        return;
    }
    write(fd, log, strlen(log));
    close(fd);
    mx_strdel(&log);
}
