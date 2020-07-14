#include "uchat_server.h"

static t_clients *create_clients() {
    t_clients *client = malloc(sizeof(t_clients));
    client->fd = 0;
    client->ssl = NULL;
    client->user_id = 0;
    client->token = NULL;
    client->channel_id = 0;
    client->index = 0;
    client->first = client;
    client->next = NULL;
    return  client;
}

int main(int argc, char **argv) {
    t_clients *client = create_clients();
    int listenfd = 0;
    struct sockaddr_in cli;
    SSL_CTX *ctx;

    mx_migration(argv[1]);
    mx_valid_check_argc_error(argc);
    SSL_library_init();
    ctx = mx_init_server_ctx();        /* initialize SSL */
    mx_load_certificates(ctx, "mycert.pem", "mycert.pem"); /* load certs */
    listenfd = mx_open_listener(atoi(argv[1]));
    while (true) {
        cli = mx_accept_connections(client, listenfd);
        client->ssl = SSL_new(ctx);              /* get new SSL state with context */
        SSL_set_fd(client->ssl, client->fd);      /* set connection socket to SSL state */
        mx_thread_create(client, cli);
        client->next = create_clients();
        client->next->first = client->first;
        client = client->next;
    }
    return 0;
}
