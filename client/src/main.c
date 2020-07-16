#include "uchat_client.h"

int main(int argc, char **argv) {
    t_mainWindowObjects mainObjects;
    SSL_CTX *ctx;

    /* Init GTK+ */
    gtk_init(&argc, &argv);

    if (argc != 3) {
        mx_printerr("uchat_server: error args\n");
        mx_printerr("example: ./uchat ip port\n");
        exit(1);
    }
    SSL_library_init();
    ctx = mx_init_ctx();
    mainObjects.fd = mx_open_connection(argv[1], atoi(argv[2]));
    mainObjects.ssl = SSL_new(ctx);
    mx_show_certs(mainObjects.ssl);
    SSL_set_fd(mainObjects.ssl, mainObjects.fd);
    mx_create_login_window(&mainObjects);
}
