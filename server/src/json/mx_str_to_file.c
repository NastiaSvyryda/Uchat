#include "uchat_server.h"

void mx_str_to_file(const char *filepath, const char *data) {
    int fd = open(filepath, O_CREAT | O_TRUNC | O_RDWR, \
                            S_IRWXO | S_IRWXU | S_IRWXG);

    if (fd != -1) {
        write(fd, data, strlen(data));
        close(fd);
    }
}
