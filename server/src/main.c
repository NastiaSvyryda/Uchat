#include "uchat_server.h"

static t_clients *create_clients() {
    t_clients *client = malloc(sizeof(t_clients));
    client->first = client;
    client->next = NULL;
    client->fd = 0;
    client->name_to = 0;
    client->name_from = 0;
    client->index = 0;
    return  client;
}

//static void get_client_name(t_clients *client) {
//    char buff[100];
//
//    memset(buff, '\0', 100);
//    read(client->fd, buff, 100);
//    mx_printstr("name recived\n");
//    client->name_from = mx_atoi(buff);
//}


int main(int argc, char **argv) {
    t_clients *client = create_clients();
    int listenfd = 0;
    struct sockaddr_in cli;

    mx_migration(argv[1]);
    mx_valid_check_argc_error(argc);
    mx_conn_list_sock(&listenfd, argv);
    while (true) {
        cli = mx_accept_connections(client, listenfd);
        client->first->index++;
//        get_client_name(client);
        mx_thread_create(client, cli);
        client->next = create_clients();
        client->next->first = client->first;
        client = client->next;
    }
}
