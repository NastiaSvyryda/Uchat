#include "uchat_client.h"

void mx_create_registre_window(struct s_MainWindowObjects* mwo) {
    GtkBuilder *builder;
    GError     *error = NULL;

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if(!gtk_builder_add_from_file( builder, "registre.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
    }
    /* Get main window pointer from UI */
    mwo->registreWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mwo->registreWindow) {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }
    mwo->entryLogin_r = GTK_ENTRY( gtk_builder_get_object(builder, "login_entry_r") );
    mwo->entryPass_r = GTK_ENTRY( gtk_builder_get_object(builder, "password_entry_r") );
    mwo->entryPass_r2 = GTK_ENTRY( gtk_builder_get_object(builder, "password_entry_r2") );
    mwo->entryName_r = GTK_ENTRY( gtk_builder_get_object(builder, "name_entry_r") );
    mwo->entrySurname_r = GTK_ENTRY( gtk_builder_get_object(builder, "surname_entry_r") );
    /* Connect signals */
    gtk_builder_connect_signals(builder, mwo);
    /* Destroy builder, since we don't need it anymore */
    g_object_unref(G_OBJECT(builder));
    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all (GTK_WIDGET(mwo->registreWindow));
    /* Start main loop */
    gtk_main();
}

