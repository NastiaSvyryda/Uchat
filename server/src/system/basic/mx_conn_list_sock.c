#include "uchat_server.h"

void mx_conn_list_sock(int *fd, char **argv) {
    struct sockaddr_in serv;

    if ((*fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        mx_printerr("uchat: couldn't create socket");
        exit(1);
    }
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(atoi(argv[1]));
    if (bind(*fd, (struct sockaddr *) &serv, sizeof(serv)) < 0) {
        mx_printerr("uchat_server: couldn't connect socket with ip or port");
        exit(1);
    }
    if (listen(*fd, 100) < 0) {
        mx_printerr("uchat_server: couldn't listen for connections");
        exit(1);
    }
}
