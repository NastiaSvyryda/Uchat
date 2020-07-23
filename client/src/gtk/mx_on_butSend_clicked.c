#include "uchat_client.h"

static char *get_text_of_textview(GtkWidget *text_view)
{
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *)text_view);
    gchar *text;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    gtk_text_buffer_delete(buffer, &start, &end);
    return text;
}

void mx_on_butSend_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    GtkWidget *mess_row;
    char *message = mx_handle_user_input(get_text_of_textview(mwo->entryMessage));
    if (!mx_validate_message(message, mwo->Window))
        return;
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    ///

    if (mwo->curr_chat_users != NULL && mwo->curr_chat != NULL) {
        t_channel channels[1] = {{
                        .channel_id = -1,
                        .user_ids_size = mx_arrlen(mwo->curr_chat_users) + 1,
                        .last_mes_time = 0}};
        strcpy(channels->channel_name, mwo->curr_chat);
        channels->user_ids = malloc(sizeof(int) * channels->user_ids_size);
        for (int i = 0; i < channels->user_ids_size; i++) {
            channels->user_ids[i] = mwo->user_ids[i];
//            puts("\n\n\nonbutsend");
//            mx_printint(channels->user_ids[i]);
//            puts("\n\n\n");
        }
        json->message.channel_id = -1;
        json->new_channel_data = channels[0];
        json->new_channel = true;
//        mwo->channel_info = mwo->channel_info->first;
//        while (mwo->channel_info->next != NULL) {
//            mwo->channel_info = mwo->channel_info->next;
//        }
//        GList *children, *iter;
//
//        children = gtk_container_get_children(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")));
//        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
//            gtk_container_remove(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), GTK_WIDGET(iter->data));
//        }
////    gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(mwo->builder, "gtkbox")), gtk_component, TRUE, TRUE, 5);
//        gtk_widget_show_all(GTK_WIDGET(mwo->Window));
        mwo->curr_messageList = gtk_list_box_new();
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->curr_messageList), GTK_SELECTION_NONE);
        gtk_container_add(GTK_CONTAINER(mwo->viewPort), mwo->curr_messageList);
        mess_row = mx_create_message(message, mwo, 0); //change signal connectors
        gtk_list_box_insert(GTK_LIST_BOX(mwo->curr_messageList), mess_row, -1);
        //gtk_widget_show_all(GTK_WIDGET(mwo->curr_messageList));
        //mx_set_chat_component(mwo, mwo->curr_messageList);
        //gtk_widget_show_all(GTK_WIDGET(mwo->chatWindow));
        gtk_widget_show_all(GTK_WIDGET(mwo->curr_messageList));
    }
    else {
        mwo->channel_info = mwo->channel_info->first;
        if (mwo->channel_info) {
            while (mwo->channel_info->next != NULL) {
                if (mwo->channel_info->channel_data.channel_id ==
                        mwo->curr_channel_info->channel_data.channel_id)
                    break;
                mwo->channel_info = mwo->channel_info->next;
            }
        }
        mess_row = mx_create_message(message, mwo, 0); //change signal connectors
        gtk_list_box_insert(GTK_LIST_BOX(mwo->channel_info->messageList), mess_row, -1);
        gtk_widget_show_all(GTK_WIDGET(mwo->Window));
//        gtk_widget_show(GTK_WIDGET(mwo->curr_channel_info->messageList));
        json->message.channel_id = mwo->channel_info->channel_data.channel_id;
    }
    json->type = JS_MES_OUT;
    json->message.text = strdup(message);
    json->user_id = mwo->user_id;
    //json->new_channel = false;
    strcpy(json->token, mwo->token);
    json->message.client1_id = mwo->user_id;

    json_str = mx_json_make_json(JS_MES_OUT, json);
    mx_printstr(json_str + 4);
    if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
    mx_strdel(&json_str);
    mx_strdel(&json->message.text);
    free(json);
    g_free(message);
}
