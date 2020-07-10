#include "uchat_server.h"

static t_clients *create_clients() {
    t_clients *client = malloc(sizeof(t_clients));

    client->fd = 0;
    client->user_id = 0;
    client->token = NULL;
    client->channel_id = 0;
    client->index = 0;
    client->next = NULL;
    client->first = client;
    return  client;
}

int main(int argc, char **argv) {
    t_clients *client = create_clients();
    int listenfd = 0;
    struct sockaddr_in cli;

    mx_migration(argv[1]);
    mx_valid_check_argc_error(argc);
    mx_conn_list_sock(&listenfd, argv);
    while (true) {
        cli = mx_accept_connections(client, listenfd);
//        get_client_name(client);
        mx_thread_create(client, cli);
        client->next = create_clients();
        client->next->first = client->first;
        client = client->next;
    }
}
