#include "uchat_client.h"

static void init_main_objects(t_mainWindowObjects *mainObjects) {
    mainObjects->ids_logins_arr = NULL;
    mainObjects->curr_chat_users = NULL;
    mainObjects->curr_chat = NULL;
    mainObjects->curr_channel_info = NULL;
    mainObjects->user_ids = NULL;
    mainObjects->channel_info = malloc(sizeof(t_channel_info));
    mainObjects->channel_info->first = mainObjects->channel_info;
    mainObjects->channel_info->next = NULL;
    mainObjects->channel_info->chat_button = NULL;
    mainObjects->channel_info->message = NULL;
    mainObjects->visibility = 1;
    mainObjects->add_chat = 0;
    mainObjects->info = 0;
    mainObjects->edit_mess = 0;
}

int main(int argc, char **argv) {
    t_mainWindowObjects mainObjects;
    SSL_CTX *ctx;
    GtkCssProvider *provider = gtk_css_provider_new ();

    init_main_objects(&mainObjects);
    gtk_css_provider_load_from_path (provider,
                                     "./client/src/gui/css/dark.css", NULL);
    gtk_init(&argc, &argv);
    gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
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
