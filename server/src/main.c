#include "uchat_server.h"

int main(int argc, char **argv) {
    t_clients *client = mx_create_client();
    client->first = client;
    int listenfd = 0;
    struct sockaddr_in cli;
    SSL_CTX *ctx;

    mx_daemon();
    mx_valid_check_argc_error(argc);
    mx_migration(argv[1]);
    SSL_library_init();
    ctx = mx_init_server_ctx();
    mx_load_certificates(ctx, mx_config_ssl_file_name(), mx_config_ssl_file_name());
    listenfd = mx_open_listener(atoi(argv[1]));
    while (true) {
        cli = mx_accept_connections(client, listenfd);
        client->ssl = SSL_new(ctx);
        SSL_set_fd(client->ssl, client->fd);
        mx_thread_create(client, cli);
        client->next = mx_create_client();
        client->next->first = client->first;
        client = client->next;
    }
    close(listenfd);
    return 0;
}
