#include "uchat_client.h"

void mx_create_login_window(t_mainWindowObjects mainObjects, int fg) {
    GtkBuilder *builder;
    GError *error = NULL;
    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if (!gtk_builder_add_from_file(builder, "login.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
    }
    g_io_add_watch(g_io_channel_unix_new((gint)fg), G_IO_IN, (GIOFunc)mx_input, &mainObjects);

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
