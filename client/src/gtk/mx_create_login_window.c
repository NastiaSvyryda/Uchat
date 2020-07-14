#include "uchat_client.h"

void mx_create_login_window(char **argv) {
    GtkBuilder *builder;
    GError *error = NULL;
    int sockfd = 0;
    SSL_CTX *ctx;
    t_mainWindowObjects mainObjects;
    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if (!gtk_builder_add_from_file(builder, "login.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
    }
    SSL_library_init();
    ctx = mx_init_ctx();
    sockfd = mx_open_connection(argv[1], atoi(argv[2]));
    mainObjects.ssl = SSL_new(ctx);
    mx_show_certs(mainObjects.ssl);
    SSL_set_fd(mainObjects.ssl, sockfd);
    g_io_add_watch(g_io_channel_unix_new((gint)sockfd), G_IO_IN, (GIOFunc)mx_input, &mainObjects);

    //END CONNECT

    /* Get main window pointer from UI */
    mainObjects.loginWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mainObjects.loginWindow)
    {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }
    mainObjects.entryLogin_l = GTK_ENTRY(gtk_builder_get_object(builder, "login_entry_l"));
    mainObjects.entryPass_l = GTK_ENTRY(gtk_builder_get_object(builder, "password_entry_l"));
    mainObjects.fd = sockfd;
    /* Connect signals */
    gtk_builder_connect_signals(builder, &mainObjects);
    /* Destroy builder, since we don't need it anymore */
    g_object_unref(G_OBJECT(builder));
    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all(GTK_WIDGET(mainObjects.loginWindow));
    /* Start main loop */
    gtk_main();

    mx_printstr("after gtk_main\n");
}
