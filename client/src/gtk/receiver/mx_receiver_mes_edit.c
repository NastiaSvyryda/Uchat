#include "uchat_client.h"

static void create_new_message(t_mainWindowObjects *mwo, t_json_data *json) {
    GList *children, *iter;
    GtkWidget *label_new;
    GtkWidget *label_login;
    GtkWidget *button;
    char *tmp = mx_strtrim_qouts(mwo->channel_info->message->login);

    mx_strdel(&mwo->channel_info->message->text);
    mwo->channel_info->message->text = mx_strdup(json->message.text);
    children = gtk_container_get_children(GTK_CONTAINER(mwo->channel_info->message->mess_row));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_container_remove(GTK_CONTAINER(mwo->channel_info->message->mess_row), GTK_WIDGET(iter->data));
    button = gtk_button_new();
    label_new = gtk_label_new(mwo->channel_info->message->text);
    gtk_label_set_line_wrap((GtkLabel *)label_new, TRUE);
    gtk_label_set_max_width_chars (GTK_LABEL (label_new), 40);
    gtk_label_set_line_wrap_mode((GtkLabel *)label_new, PANGO_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(button), label_new);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_name (button, "left_button");
    label_login = gtk_label_new(tmp);//
    gtk_widget_set_name (label_login, "left_label");
    gtk_container_add(GTK_CONTAINER(mwo->channel_info->message->mess_row), label_login);//
    gtk_widget_set_halign(label_login, GTK_ALIGN_START);//
    gtk_container_add(GTK_CONTAINER(mwo->channel_info->message->mess_row), button);
    mx_strdel(&tmp);
}

void mx_receiver_mes_edit(t_mainWindowObjects *mwo, t_json_data *json) {
    mwo->channel_info = mwo->channel_info->first;
    while (mwo->channel_info->next != NULL) {
        if (mwo->channel_info->channel_data.channel_id ==
            json->message.channel_id)
            break;
        mwo->channel_info = mwo->channel_info->next;
    }
    if (mwo->channel_info->message != NULL) {
        mwo->channel_info->message = mwo->channel_info->message->first;
        while (mwo->channel_info->message->next != NULL) {
            if (mwo->channel_info->message->message_id ==
                json->message.message_id)
                break;
            mwo->channel_info->message = mwo->channel_info->message->next;
        }
        create_new_message(mwo, json);
        gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    }
}

