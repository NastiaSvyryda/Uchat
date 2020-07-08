#include "uchat_client.h"

int main(int argc, char **argv) {
    /* Init GTK+ */
    gtk_init(&argc, &argv);

    if (argc != 3) {
        mx_printerr("uchat_server: error args\n");
        mx_printerr("example: ./uchat ip port\n");
        exit(1);
    }

    mx_create_login_window(argv);
}
