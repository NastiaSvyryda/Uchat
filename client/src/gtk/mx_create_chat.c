#include "uchat_client.h"

void mx_on_chat_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_channel_info *channel_info = malloc(sizeof(t_channel_info));
//    int channel_id = 0;

    channel_info = g_object_get_data(G_OBJECT(button),(gchar *)"channel_id");

    //gtk_label_set_text(GTK_LABEL(mwo->label_chat),);
    char *json_str = NULL;
    t_json_data json = {.user_id = mwo->user_id, .message.last_message_id = -1, .type = JS_MES_HIST};
    puts("\n");
    mx_printstr(channel_info->channel_data.channel_name);
//    mx_printint(channel_info->channel_data.channel_id);
    puts("\n");

    strcpy(json.token, mwo->token);
//    mwo->channel_info = mwo->channel_info->first;
//    while (mwo->channel_info->next->next != NULL) {
//        if (mwo->channel_info->chat_button == button)
//            break;
//        mwo->channel_info = mwo->channel_info->next;
//    }
    json.message.channel_id = channel_info->channel_data.channel_id;
    json_str = mx_json_make_json(JS_MES_HIST, &json);
    mx_printstr(json_str + 4);
    if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
    mx_set_component(mwo, mwo->chatWindow);
}

GtkWidget *mx_create_chat(const gchar *text, struct s_MainWindowObjects *mwo)
{
    GtkWidget *button;
    GtkWidget *chat_row;

    chat_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    button = gtk_button_new_with_label(text);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(chat_row), button);
    g_signal_connect(button, "clicked", G_CALLBACK(mx_on_chat_clicked), mwo);
    mx_printint(mwo->channel_info->channel_data.channel_id);
    puts("\n");
    g_object_set_data(G_OBJECT(button), (gchar *)"channel_id",mwo->channel_info);
    return chat_row;
}
