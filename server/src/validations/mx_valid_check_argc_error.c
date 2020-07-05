#include "uchat_server.h"

void mx_valid_check_argc_error(int argc) {
    if (argc != 2) {
        mx_printerr("uchat_server: error args\n");
        mx_printerr("example: ./uchat_server port\n");
        exit(1);
    }
}
