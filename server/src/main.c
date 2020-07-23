#include "uchat_server.h"

int main(int argc, char **argv) {
    t_main main;
    main.client = mx_create_client();
    main.client->first = main.client;
    main.wait = malloc(sizeof(t_wait));
    main.wait->next = NULL;
    main.wait->json_str = NULL;
    main.wait->first = main.wait;
    int listenfd = 0;
    struct sockaddr_in cli;
    SSL_CTX *ctx;

    //mx_daemon();
    mx_valid_check_argc_error(argc);
    mx_migration(argv[1]);
    SSL_library_init();
    ctx = mx_init_server_ctx();/* initialize SSL */
    mx_load_certificates(ctx, mx_config_ssl_file_name(), mx_config_ssl_file_name()); /* load certs */
    listenfd = mx_open_listener(atoi(argv[1]));
    while (true) {
        cli = mx_accept_connections(&main, listenfd);
        main.client->ssl = SSL_new(ctx);/* get new SSL state with context */
        SSL_set_fd(main.client->ssl, main.client->fd);/* set connection socket to SSL state */
        mx_thread_create(&main, cli);
        main.client->next = mx_create_client();
        main.client->next->first = main.client->first;
        main.client = main.client->next;
    }
    close(listenfd);
    return 0;
}
