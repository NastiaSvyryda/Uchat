#include "uchat_client.h"


////
static void
row_unrevealed(GObject *revealer, GParamSpec *pspec, gpointer data) {
    GtkWidget *row, *list;
    if (pspec == NULL || data == NULL)
        puts("NULL\n");
    row = gtk_widget_get_parent(GTK_WIDGET(revealer));
    list = gtk_widget_get_parent(row);

    gtk_container_remove(GTK_CONTAINER(list), row);
}

void
remove_this_row(GtkButton *button, GtkWidget *child) {
    GtkWidget *row, *revealer;
    if (button == NULL)
        puts("NULL\n");
    row = gtk_widget_get_parent(child);
    revealer = gtk_revealer_new();
    gtk_revealer_set_reveal_child(GTK_REVEALER(revealer), TRUE);
    gtk_widget_show(revealer);
    G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    gtk_widget_reparent(child, revealer);
    G_GNUC_END_IGNORE_DEPRECATIONS
    gtk_container_add(GTK_CONTAINER (row), revealer);
    g_signal_connect(revealer, "notify::child-revealed",
                     G_CALLBACK(row_unrevealed), NULL);
    gtk_revealer_set_reveal_child(GTK_REVEALER(revealer), FALSE);
}


void
row_revealed(GObject *revealer, GParamSpec *pspec, gpointer data) {
    GtkWidget *row, *child;
    if (pspec == NULL || data == NULL)
        puts("NULL\n");
    row = gtk_widget_get_parent(GTK_WIDGET(revealer));
    child = gtk_bin_get_child(GTK_BIN(revealer));
    g_object_ref(child);
    gtk_container_remove(GTK_CONTAINER(revealer), child);
    gtk_widget_destroy(GTK_WIDGET(revealer));
    gtk_container_add(GTK_CONTAINER(row), child);
    g_object_unref(child);
}

void
mx_add_chat(__attribute__((unused))GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    GtkWidget *row;
    gchar *text;

    //list = gtk_widget_get_parent(gtk_widget_get_parent(mwo->row));
    text = g_strdup_printf("message 2");
    row = mx_create_row(text, mwo);//change signal connectors
    gtk_list_box_insert(GTK_LIST_BOX(mwo->list), row, -1);
    gtk_widget_show_all(GTK_WIDGET(mwo->mainWindow));
}

void
mx_add_message(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    GtkWidget *row;
    char *message = (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryMessage));
//    if (button == NULL)
//        puts("NULL\n");
    row = mx_create_row(message, mwo);//change signal connectors
    gtk_list_box_insert(GTK_LIST_BOX(mwo->messageList), row, -1);
    gtk_widget_show_all(GTK_WIDGET(mwo->chatWindow));
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_MES_OUT;
    json->message.text = strdup(message);
    json->message.client1_id = 1;
    json->message.channel_id= 3;
    json_str = mx_json_make_json(JS_MES_OUT, json);
    write(mwo->fd, json_str, mx_strlen(json_str + 4) + 4);
    mx_strdel(&json_str);
    mx_strdel(&json->message.text);
    free(json);
}

static void
add_separator(GtkListBoxRow *row, GtkListBoxRow *before, gpointer data) {
    if (!before)
        return;
    if (data == NULL)
        puts("NULL\n");
    gtk_list_box_row_set_header(row, gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
}

void mx_create_chat_window(struct s_MainWindowObjects *mwo) {
    GtkBuilder *builder;
    GError *error = NULL;
    GtkWidget *list = NULL;
    GtkWidget *row = NULL;
    gint i;
    gchar *text;
    //GtkWidget *view = NULL;

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if (!gtk_builder_add_from_file(builder, "chat.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
    }
    /* Get main window pointer from UI */
    mwo->chatWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mwo->chatWindow) {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }
    mwo->entryMessage = GTK_ENTRY(gtk_builder_get_object(builder, "message_entry"));
    ///
    list = (mwo->messageList = gtk_list_box_new());
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list), GTK_SELECTION_NONE);
    //gtk_list_box_set_header_func (GTK_LIST_BOX (list), add_separator, NULL, NULL);
    gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(builder, "scrolled_chat")), list);
    for (i = 0; i < 5; i++) {
        text = g_strdup_printf("message %d", i);
        row = mx_create_row(text, mwo);//change signal connectors
        gtk_list_box_insert(GTK_LIST_BOX(list), row, -1);
    }
    ///
    //view = mx_create_view_and_model();
    //gtk_container_add (GTK_CONTAINER (gtk_builder_get_object(builder, "messages_tree")), view);
    /* Connect signals */
    gtk_builder_connect_signals(builder, mwo);
    /* Destroy builder, since we don't need it anymore */
    g_object_unref(G_OBJECT(builder));
    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all(GTK_WIDGET(mwo->chatWindow));
    /* Start main loop */
    gtk_main();

}

void mx_on_chat_clicked(GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;

    if (button == NULL && data == NULL)
        puts("NULL");
    gtk_window_close(mwo->mainWindow);
    mx_create_chat_window(mwo);
}
GtkWidget *mx_create_row(const gchar *text, struct s_MainWindowObjects *mwo) {
    GtkWidget *button;

    mwo->row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    button = gtk_button_new_with_label(text);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(mwo->row), button);
    g_signal_connect(button, "clicked", G_CALLBACK(mx_on_chat_clicked), mwo);
//    g_signal_connect (button, "clicked", G_CALLBACK (remove_this_row), row);
//    button = gtk_button_new_with_label ("+");
//    gtk_widget_set_valign (button, GTK_ALIGN_CENTER);
//    gtk_container_add (GTK_CONTAINER (row), button);
//    g_signal_connect (button, "clicked", G_CALLBACK (add_row_below), row);

    return mwo->row;
}

void mx_create_main_window(struct s_MainWindowObjects *mwo) {
    GtkBuilder *builder;
    GError *error = NULL;
    GtkWidget *list = NULL;
    GtkWidget *row = NULL;
    gint i;
    gchar *text;
    //GtkWidget *view = NULL;

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if (!gtk_builder_add_from_file(builder, "menu.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
    }
    /* Get main window pointer from UI */
    mwo->mainWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mwo->mainWindow) {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }
    ///
    list = (mwo->list = gtk_list_box_new());
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list), GTK_SELECTION_NONE);
    gtk_list_box_set_header_func(GTK_LIST_BOX(list), add_separator, NULL, NULL);
    gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(builder, "scrolled_window_chats")), list);
    for (i = 0; i < 5; i++) {
        text = g_strdup_printf("chat %d", i);
        row = mx_create_row(text, mwo);
        gtk_list_box_insert(GTK_LIST_BOX(list), row, -1);
    }
    text = g_strdup_printf("message 2");
    row = mx_create_row(text, mwo);//change signal connectors
    gtk_list_box_insert(GTK_LIST_BOX(mwo->list), row, -1);
    ///
    //view = mx_create_view_and_model();
    //gtk_container_add (GTK_CONTAINER (gtk_builder_get_object(builder, "messages_tree")), view);
    /* Connect signals */
    gtk_builder_connect_signals(builder, mwo);
    /* Destroy builder, since we don't need it anymore */
    g_object_unref(G_OBJECT(builder));
    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all(GTK_WIDGET(mwo->mainWindow));

    gtk_main();
}
