#include "uchat_server.h"

static t_clients *create_clients() {
    t_clients *client = malloc(sizeof(t_clients));
    client->fd = 0;
    client->user_id = 0;
    return client;
}

static void conn_list_sock(int *fd, char **argv) {
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
    mx_printint(*fd);
}

static struct sockaddr_in accept_connections(t_clients *client, int listenfd) {
    struct sockaddr_in cli;
    int cli_size = sizeof(cli);

    if ((client->fd = accept(listenfd, (struct sockaddr *)&cli, (socklen_t *) &cli_size)) == -1) {
        mx_printerr("uchat_server: error accepting connection on a socket");
        exit(1);
    }
    return cli;
}

int main(int argc, char **argv) {
    t_clients *client = create_clients();
    int listenfd = 0;
    struct sockaddr_in cli;

    migration(argv[1]);
    mx_valid_check_argc_error(argc);
    conn_list_sock(&listenfd, argv);
    while (true) {
        cli = accept_connections(client, listenfd);

    }
    return 0;
}
