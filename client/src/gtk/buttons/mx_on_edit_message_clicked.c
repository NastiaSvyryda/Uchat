#include "uchat_client.h"

void mx_cancel_edit_clicked (__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mwo->edit_mess = 0;
    gtk_widget_destroy(mwo->change_mess_Dialog);
}

static void edit_mess_request(t_mainWindowObjects *mwo, char *message) {
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_MES_EDIT_OUT;
    json->message.text = message;
    json->message.message_id = mwo->curr_channel_info->message->message_id;
    json->message.client1_id = mwo->user_id;
    json->user_id = mwo->user_id;
    strcpy(json->token, mwo->token);
    json_str = mx_json_make_json(JS_MES_EDIT_OUT, json);
    if (SSL_connect(mwo->ssl) == -1)
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, *(int *) json_str + 4);
    mx_strdel(&json_str);
    mx_strdel(&json->message.text);
    free(json);
}

static void edit_button(t_mainWindowObjects *mwo, char *message) {
    GtkWidget *label_new;
    GtkWidget *button;

    button = gtk_button_new();
    label_new = gtk_label_new(message);
    gtk_label_set_line_wrap((GtkLabel *)label_new, TRUE);
    gtk_label_set_max_width_chars (GTK_LABEL (label_new), 40);
    gtk_label_set_line_wrap_mode((GtkLabel *)label_new, PANGO_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(button), label_new);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_name (button, "right_button");
    gtk_container_add(
            GTK_CONTAINER(mwo->curr_channel_info->message->mess_row),
            button);
    gtk_widget_set_halign(mwo->curr_channel_info->message->mess_row,
                          GTK_ALIGN_END);
    g_object_set_data(G_OBJECT(button), (gchar *) "message_info",
                      mwo->curr_channel_info->message);
    g_signal_connect(button, "button-press-event",
                     G_CALLBACK(mx_on_message_clicked), mwo);
}

static void delete_old_button(t_mainWindowObjects *mwo, char *message) {
    GList *children;
    GList *iter;

    mx_strdel(&mwo->curr_channel_info->message->text);
    mwo->curr_channel_info->message->text = mx_strdup(message);
    children = gtk_container_get_children(
            GTK_CONTAINER(mwo->curr_channel_info->message->mess_row));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_container_remove(
                GTK_CONTAINER(mwo->curr_channel_info->message->mess_row),
                GTK_WIDGET(iter->data));
    }
}

void mx_on_edit_message_clicked(__attribute__((unused)) GtkWidget *clicked, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *tmp_mess = mx_strdup((char *)mx_get_text_of_textview(mwo->entry_edit_message));
    char *message = mx_handle_user_input(tmp_mess);
    t_message_list *curr_message_info = malloc(sizeof(t_message_list));

    if (mx_validate_message(message, mwo->change_mess_Dialog)) {
        curr_message_info = g_object_get_data(G_OBJECT(clicked),
                                              (gchar *) "message_info");
        mwo->curr_channel_info->message = mwo->curr_channel_info->message->first;
        while (mwo->curr_channel_info->message->next != NULL) {
            if (mwo->curr_channel_info->message->message_id ==
                curr_message_info->message_id)
                break;
            mwo->curr_channel_info->message = mwo->curr_channel_info->message->next;
        }
        delete_old_button(mwo, tmp_mess);
        edit_button(mwo, tmp_mess);
        mwo->edit_mess = 0;
        gtk_widget_destroy(mwo->change_mess_Dialog);
        gtk_widget_show_all(GTK_WIDGET(mwo->Window));
        edit_mess_request(mwo, message);
        mx_strdel(&tmp_mess);
    }
}

