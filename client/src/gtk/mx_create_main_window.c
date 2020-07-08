#include "uchat_client.h"

static void *reciever(void *mwo_strct) {
    struct s_MainWindowObjects *mwo = (struct s_MainWindowObjects *)mwo_strct;
    char lin[4];
    int len = 0;
    t_json_data *json = calloc(1, sizeof(t_json_data));
    char *json_str = NULL;
    read(mwo->fd, lin, 4);
    len = lin[0];
    json_str = mx_strnew(len);
    read(mwo->fd, json_str, 100);
    mx_printstr(json_str);
    json = mx_json_parse(json_str);
    mx_printstr(json->message.text);
    mx_strdel(&json_str);
    mx_strdel(&json->message.text);
    return NULL;
}


void mx_create_main_window(struct s_MainWindowObjects *mwo) {
    GtkBuilder *builder;
    GError     *error = NULL;
    GtkWidget *view = NULL;

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if(!gtk_builder_add_from_file( builder, "menu.glade", &error)) {
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
    view = mx_create_view_and_model();
    gtk_container_add (GTK_CONTAINER (gtk_builder_get_object(builder, "messages_tree")), view);
    /* Connect signals */
    gtk_builder_connect_signals(builder, mwo);
    /* Destroy builder, since we don't need it anymore */
    g_object_unref(G_OBJECT(builder));
    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all (GTK_WIDGET(mwo->mainWindow));
    pthread_t thread = NULL;

    if ((pthread_create(&thread, NULL,reciever, mwo)) != 0) {
        mx_printerr("uchat_server: thread creating error");
        exit(1);
    }


    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_MES_OUT;
    json->message.text = strdup("hallo");
    json->message.client1_id = 1;
    json->message.client2_id = 2;
    json_str = mx_json_make_json(JS_MES_OUT, json);
    write(mwo->fd, json_str, mx_strlen(json_str + 4) + 4);
    mx_strdel(&json_str);
    mx_strdel(&json->message.text);
    free(json);


//    char lin[4];
//    int len = 0;
//    json = calloc(1, sizeof(t_json_data));
////    char *json_str;
//    read(mwo->fd, lin, 4);
//    len = lin[0];
//    json_str = mx_strnew(len);
//    read(mwo->fd, json_str, 100);
//    mx_printstr(json_str);
//    json = mx_json_parse(json_str);
//    mx_printstr(json->message.text);
//    mx_strdel(&json_str);
//    mx_strdel(&json->message.text);
    /* Start main loop */
    gtk_main();

}
