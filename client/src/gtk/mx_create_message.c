#include "uchat_client.h"

void mx_on_message_clicked(__attribute__((unused)) GtkWidget *button, GdkEventButton *event, __attribute__((unused))gpointer data) {
    //t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    //char *json_str = NULL;
    //t_json_data *json = calloc(1, sizeof(t_json_data));

    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
        GtkBuilder *builder;
        GError *error = NULL;
        t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

        t_message_list *curr_message_info = malloc(sizeof(t_message_list));//clean

        curr_message_info = g_object_get_data(G_OBJECT(button),(gchar *)"message_info");
        mwo->curr_channel_info->message = mwo->curr_channel_info->message->first;
        //if (mwo->channel_info->message) {
        while (mwo->curr_channel_info->message->next != NULL) {
            if (mwo->curr_channel_info->message->message_id ==
                curr_message_info->message_id)
                break;
            mwo->curr_channel_info->message = mwo->curr_channel_info->message->next;
        }

        builder = gtk_builder_new();
//    gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
//                                               GTK_STYLE_PROVIDER(mainObjects->provider),
//                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
        if (!gtk_builder_add_from_file(builder, "sent_gui.glade", &error)) {
            mx_printstr(error->message);
        }
        mwo->change_mess_Dialog = GTK_WIDGET(
                gtk_builder_get_object(builder, "dialog_change_message"));
        mwo->entry_edit_message = GTK_WIDGET(
                gtk_builder_get_object(builder, "edite_message_text_view"));
        GtkTextBuffer *buffer = gtk_text_buffer_new (NULL);//gtk_text_view_get_buffer((GtkTextView *)mwo->entry_edit_message);
        gtk_text_buffer_set_text (buffer, mwo->curr_channel_info->message->text, (gint)mx_strlen(mwo->curr_channel_info->message->text));
        gtk_text_view_set_buffer ((GtkTextView *)mwo->entry_edit_message, buffer);
        g_object_set_data(gtk_builder_get_object(builder, "apply_change_message"), (gchar *)"message_info", mwo->curr_channel_info->message);
        g_object_set_data(gtk_builder_get_object(builder, "delete_message_button"), (gchar *)"message_info", mwo->curr_channel_info->message);
        gtk_builder_connect_signals(builder, mwo);
        gtk_window_set_transient_for(GTK_WINDOW(mwo->change_mess_Dialog),
                                     mwo->Window);
        gtk_window_set_position(GTK_WINDOW(mwo->change_mess_Dialog),
                                GTK_WIN_POS_CENTER_ON_PARENT);
        //обработать ответ от сервера с логинами в reciever
        gtk_widget_show_all(GTK_WIDGET(mwo->change_mess_Dialog));
//        json->type = JS_MES_EDIT_OUT;
//        json->message.text = strdup("bye");
//        json->message.message_id = 73;
//        json->message.client1_id = mwo->user_id;
//        strcpy(json->token, mwo->token);
//        json_str = mx_json_make_json(JS_MES_EDIT_OUT, json);
//
//        //mx_printint(*(int *)json_str + 4);
//        if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
//            ERR_print_errors_fp(stderr);
//        else
//            SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
//        mx_strdel(&json_str);
//        mx_strdel(&json->message.text);
    }
    //free(json);
}

void mx_cancel_edit_message_clicked (__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    gtk_widget_destroy(mwo->change_mess_Dialog);
}

void mx_on_edit_message_clicked(__attribute__((unused)) GtkWidget *clicked, gpointer data) {//delete child and replace button
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_message_list *curr_message_info = malloc(sizeof(t_message_list));
    char *message = mx_handle_user_input(
            mx_get_text_of_textview(mwo->entry_edit_message));
    GList *children, *iter;
    GtkWidget *label_new;
    GtkWidget *button;
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    curr_message_info = g_object_get_data(G_OBJECT(clicked),(gchar *)"message_info");
    mwo->curr_channel_info->message = mwo->curr_channel_info->message->first;
    //if (mwo->channel_info->message) {
    while (mwo->curr_channel_info->message->next != NULL) {
        if (mwo->curr_channel_info->message->message_id ==
            curr_message_info->message_id)
            break;
        mwo->curr_channel_info->message = mwo->curr_channel_info->message->next;
    }
    mx_strdel(&mwo->curr_channel_info->message->text);
    mwo->curr_channel_info->message->text = mx_strdup(message);
    children = gtk_container_get_children(GTK_CONTAINER(mwo->curr_channel_info->message->mess_row));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_container_remove(GTK_CONTAINER(mwo->curr_channel_info->message->mess_row), GTK_WIDGET(iter->data));
    }
    button = gtk_button_new();
    label_new = gtk_label_new(message);
    gtk_label_set_line_wrap((GtkLabel *)label_new, TRUE);
    gtk_label_set_line_wrap_mode((GtkLabel *)label_new, PANGO_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(button), label_new);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(mwo->curr_channel_info->message->mess_row), button);
    gtk_widget_set_halign(mwo->curr_channel_info->message->mess_row, GTK_ALIGN_END);
    g_object_set_data(G_OBJECT(button), (gchar *)"message_info", mwo->curr_channel_info->message);
    g_signal_connect(button, "button-press-event", G_CALLBACK(mx_on_message_clicked), mwo);
    gtk_widget_destroy(mwo->change_mess_Dialog);
    gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    //
    json->type = JS_MES_EDIT_OUT;
    json->message.text = message;
    json->message.message_id = mwo->curr_channel_info->message->message_id;
    json->message.client1_id = mwo->user_id;
    json->user_id = mwo->user_id;
    strcpy(json->token, mwo->token);
    json_str = mx_json_make_json(JS_MES_EDIT_OUT, json);
    //mx_printint(*(int *)json_str + 4);
    if (SSL_connect(mwo->ssl) == -1)  /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
    mx_strdel(&json_str);
    mx_strdel(&json->message.text);

    //mx_strdel(&message);
    free(json);
}

void mx_on_delete_message_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_message_list *curr_message_info = malloc(sizeof(t_message_list));

    curr_message_info = g_object_get_data(G_OBJECT(button),(gchar *)"message_info");
    mwo->curr_channel_info->message = mwo->curr_channel_info->message->first;
    //if (mwo->channel_info->message) {
    while (mwo->curr_channel_info->message->next != NULL) {
        if (mwo->curr_channel_info->message->message_id ==
            curr_message_info->message_id)
            break;
        mwo->curr_channel_info->message = mwo->curr_channel_info->message->next;
    }
    gtk_widget_destroy(mwo->curr_channel_info->message->mess_row);
    gtk_widget_destroy(mwo->change_mess_Dialog);
    gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    //}
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));
    json->message.message_id = mwo->curr_channel_info->message->message_id;
    json->user_id = mwo->user_id;
    json->message.channel_id = mwo->curr_channel_info->channel_data.channel_id;
    strcpy(json->token, mwo->token);
    json_str = mx_json_make_json(JS_MES_DEL_OUT, json);
    //mx_printint(*(int *)json_str + 4);
    if (SSL_connect(mwo->ssl) == -1)  /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
    mx_strdel(&json_str);
    free(json);

}


GtkWidget *mx_create_message(const gchar *text, struct s_MainWindowObjects *mwo, int align) {
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *row;
    GtkWidget *label_login;//
    char *tmp = NULL;

    row = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    button = gtk_button_new();
    label = gtk_label_new(text);
    gtk_label_set_line_wrap((GtkLabel *)label, TRUE);
    gtk_label_set_line_wrap_mode((GtkLabel *)label, PANGO_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(button), label);
//    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    if (align) {
        tmp = mx_strtrim_qouts(mwo->channel_info->message->login);
        label_login = gtk_label_new(tmp);//
        gtk_container_add(GTK_CONTAINER(row), label_login);//
        gtk_widget_set_halign(label_login, GTK_ALIGN_START);//

        gtk_widget_set_halign(row, GTK_ALIGN_START);
    }
    else {
        gtk_widget_set_halign(row, GTK_ALIGN_END);
        g_object_set_data(G_OBJECT(button), (gchar *)"message_info", mwo->channel_info->message);
        g_signal_connect(button, "button-press-event", G_CALLBACK(mx_on_message_clicked), mwo);
    }
    gtk_container_add(GTK_CONTAINER(row), button);
    return row;
}
