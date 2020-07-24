#include "uchat_client.h"

void mx_on_chat_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{ // почистить при выходе из чата
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_channel_info *curr_channel_info = malloc(sizeof(t_channel_info));
//    int channel_id = 0;
    curr_channel_info = g_object_get_data(G_OBJECT(button),(gchar *)"channel_id");
    mwo->channel_info = mwo->channel_info->first;
    if (mwo->channel_info) {
        while (mwo->channel_info->next != NULL) {
            if (mwo->channel_info->channel_data.channel_id ==
                curr_channel_info->channel_data.channel_id)
                break;
            mwo->channel_info = mwo->channel_info->next;
        }
    }
    else
       puts("ERR mx_on_chat_clicked");
    mwo->curr_channel_info = mwo->channel_info;
    //gtk_label_set_text(GTK_LABEL(mwo->label_chat),);
    if (mwo->channel_info->message == NULL) {
        char *json_str = NULL;
        t_json_data json = {.user_id = mwo->user_id, .message.last_message_id = -1, .type = JS_MES_HIST};
        puts("\n");
        mx_printstr(mwo->channel_info->channel_data.channel_name);
//    mx_printint(channel_info->channel_data.channel_id);
        puts("\n");

        strcpy(json.token, mwo->token);
        json.message.channel_id = mwo->channel_info->channel_data.channel_id;
        json_str = mx_json_make_json(JS_MES_HIST, &json);
        mx_printstr(json_str + 4);
        if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
            ERR_print_errors_fp(stderr);
        else
            SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
//    mwo->curr_channel_info->messageList = gtk_list_box_new();
//    gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->curr_channel_info->messageList), GTK_SELECTION_NONE);
//    gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), mwo->curr_channel_info->messageList);
    }
//    GList *children, *iter;
//
//    children = gtk_container_get_children(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")));
//    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
//        gtk_container_remove(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), GTK_WIDGET(iter->data));
//    }
////    gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(mwo->builder, "gtkbox")), gtk_component, TRUE, TRUE, 5);
//    gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    //GtkWidget *viewPort = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "scrolled_chat"));

    //mx_set_component(mwo, mwo->chatWindow);
    GtkWidget *viewPort = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "scrolled_chat"));
    GtkWidget *scrolled_window = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "chatScrolledWindow"));

    gtk_container_add(GTK_CONTAINER(viewPort), mwo->channel_info->messageList);

    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *)scrolled_window);
    gtk_adjustment_set_value(adjustment, gtk_adjustment_get_upper(adjustment));
    gtk_scrolled_window_set_vadjustment((GtkScrolledWindow *)scrolled_window, (GtkAdjustment *)adjustment);
    mx_set_component(mwo, mwo->chatWindow);
}

GtkWidget *mx_create_chat(const gchar *text, struct s_MainWindowObjects *mwo)
{
    GtkWidget *button;
    GtkWidget *chat_row;

    chat_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    button = gtk_button_new_with_label(text);
//    gtk_widget_set_hexpand(chat_row, TRUE);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(chat_row), button);
    g_signal_connect(button, "clicked", G_CALLBACK(mx_on_chat_clicked), mwo);
//    mx_printint(mwo->channel_info->channel_data.channel_id);
//    puts("\n");
    g_object_set_data(G_OBJECT(button), (gchar *)"channel_id",mwo->channel_info);
    return chat_row;
}
