#include "uchat_client.h"

void mx_back_to_chats(__attribute__((unused))GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    
    mx_set_component(mwo, mwo->mainWindow);
}

void mx_add_chat(__attribute__((unused))GtkWidget *button, gpointer data) {//создавать новое диалоговое окно
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    char *json_str = NULL;
    t_json_data json = {.user_id = mwo->user_id, .type = JS_GET_USERS};
    strcpy(json.token, mwo->token);
    json_str = mx_json_make_json(JS_GET_USERS, &json);
    mx_printstr(json_str + 4);
    SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
    mx_strdel(&json_str);

    gtk_widget_show_all(mwo->addChat_Dialog);
    gtk_dialog_run(GTK_DIALOG(mwo->addChat_Dialog));
}

char *get_text_of_textview(GtkWidget *text_view) {
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *)text_view);
    gchar *text;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    gtk_text_buffer_delete(buffer, &start, &end);
    return text;
}

void mx_add_message(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    GtkWidget *row;
    char *message = get_text_of_textview(mwo->entryMessage);
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    row = mx_create_message(message, mwo, 0);//change signal connectors
    gtk_list_box_insert(GTK_LIST_BOX(mwo->messageList), row, -1);
    gtk_widget_show_all(GTK_WIDGET(mwo->chatWindow));
    ///
    int user_ids1[3] = {1, 2, 3};
    t_channel channels[1] = {
            {
                    .channel_id = 0,
                    .channel_name = "asdasdasdddsdsdas",
                    .user_ids = user_ids1,
                    .user_ids_size = 3,
                    .last_mes_time = 0
            }
    };
    json->type = JS_MES_OUT;
    json->message.text = strdup(message);
    json->user_id = mwo->user_id;
    json->new_channel = true;
    json->new_channel_data = channels[0];
    strcpy(json->token, mwo->token);
    json->message.client1_id = mwo->user_id;
    json->message.channel_id= 1;


    json_str = mx_json_make_json(JS_MES_OUT, json);
    if ( SSL_connect(mwo->ssl) == -1 )   /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
    mx_strdel(&json_str);
    mx_strdel(&json->message.text);
    free(json);
    g_free(message);
}

//static void add_separator(GtkListBoxRow *row, GtkListBoxRow *before, gpointer data) {
//    if (!before)
//        return;
//    if (data == NULL)
//        puts("NULL\n");
//    gtk_list_box_row_set_header(row, gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
//}

void mx_on_chat_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;

    mx_set_component(mwo, mwo->chatWindow);
}

void mx_on_message_clicked(__attribute__((unused))GtkWidget *button, GdkEventButton *event, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {
        json->type = JS_MES_EDIT_OUT;
        json->message.text = strdup("bye");
        json->message.message_id = 73;
        json->message.client1_id = mwo->user_id;
        strcpy(json->token, mwo->token);
        json_str = mx_json_make_json(JS_MES_EDIT_OUT, json);

        mx_printint(*(int*)json_str);
        if ( SSL_connect(mwo->ssl) == -1 )   /* perform the connection */
            ERR_print_errors_fp(stderr);
        else
            SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
        mx_strdel(&json_str);
        mx_strdel(&json->message.text);
    }
    free(json);

}

GtkWidget *mx_create_chat(const gchar *text, struct s_MainWindowObjects *mwo) {
    GtkWidget *button;

    mwo->row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    button = gtk_button_new_with_label(text);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(mwo->row), button);
    g_signal_connect(button, "clicked", G_CALLBACK(mx_on_chat_clicked), mwo);
    return mwo->row;
}

GtkWidget *mx_create_message(const gchar *text, struct s_MainWindowObjects *mwo, int align) {
    GtkWidget *button;

    mwo->mess_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    if (align)
        gtk_widget_set_halign(mwo->mess_row, GTK_ALIGN_START);
    else
        gtk_widget_set_halign(mwo->mess_row, GTK_ALIGN_END);

    button = gtk_button_new_with_label(text);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(mwo->mess_row), button);
    g_signal_connect(button, "button-press-event", G_CALLBACK(mx_on_message_clicked), mwo);
    return mwo->mess_row;
}

