#include "uchat_server.h"

struct sockaddr_in mx_accept_connections(t_clients *client, int listenfd) {
    struct sockaddr_in cli;
    int cli_size = sizeof(cli);

    if ((client->fd = accept(listenfd, (struct sockaddr *)&cli, (socklen_t *) &cli_size)) == -1) {
        mx_printerr(
                "uchat_server: error accepting connection on a socket");
        exit(1);
    }
    return cli;
}
