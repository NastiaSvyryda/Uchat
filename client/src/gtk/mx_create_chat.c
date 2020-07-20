#include "uchat_client.h"

void mx_on_chat_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{ // почистить при выходе из чата
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    mwo->curr_channel_info = malloc(sizeof(t_channel_info));
//    int channel_id = 0;

    mwo->curr_channel_info = g_object_get_data(G_OBJECT(button),(gchar *)"channel_id");

    //gtk_label_set_text(GTK_LABEL(mwo->label_chat),);
    char *json_str = NULL;
    t_json_data json = {.user_id = mwo->user_id, .message.last_message_id = -1, .type = JS_MES_HIST};
    puts("\n");
    mx_printstr(mwo->curr_channel_info->channel_data.channel_name);
//    mx_printint(channel_info->channel_data.channel_id);
    puts("\n");

    strcpy(json.token, mwo->token);
    json.message.channel_id = mwo->curr_channel_info->channel_data.channel_id;
    json_str = mx_json_make_json(JS_MES_HIST, &json);
    mx_printstr(json_str + 4);
    if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
//    mwo->curr_channel_info->messageList = gtk_list_box_new();
//    gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->curr_channel_info->messageList), GTK_SELECTION_NONE);
//    gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), mwo->curr_channel_info->messageList);
    mx_set_component(mwo, mwo->curr_channel_info->chatWindow);
    gtk_widget_show_all(GTK_WIDGET(mwo->curr_channel_info->messageList));
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
//    mx_printint(mwo->channel_info->channel_data.channel_id);
//    puts("\n");
    mwo->channel_info->chatWindow = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "chat_box"));
    g_object_set_data(G_OBJECT(button), (gchar *)"channel_id",mwo->channel_info);
    return chat_row;
}
