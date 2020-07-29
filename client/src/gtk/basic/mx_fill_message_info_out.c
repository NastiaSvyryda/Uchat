#include "uchat_client.h"

static void fill_new_chat(t_mainWindowObjects *mwo, t_json_data *json) {
    strcpy(mwo->channel_info->channel_data.channel_name, mwo->curr_chat);
    mwo->channel_info->channel_data.channel_id = json->message.channel_id;
    mwo->channel_info->channel_data.last_mes_time = json->message.delivery_time;
    mwo->channel_info->channel_data.user_ids_size = mx_arrlen(mwo->curr_chat_users) + 1;
    mwo->channel_info->channel_data.user_ids = malloc(sizeof(int) * mwo->channel_info->channel_data.user_ids_size);
    for (int i = 0; i < mwo->channel_info->channel_data.user_ids_size; i++)
        mwo->channel_info->channel_data.user_ids[i] = mwo->user_ids[i];
    mwo->channel_info->message = NULL;
    mwo->channel_info->messageList = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->channel_info->messageList), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(mwo->viewPort), mwo->channel_info->messageList);
    mwo->channel_info->next = malloc(sizeof(t_channel_info));
    mwo->channel_info->next->first = mwo->channel_info->first;
    mwo->channel_info->next->chat_button = NULL;
    mwo->channel_info->next->next = NULL;
    mwo->channel_info->next->message = NULL;
    mwo->channel_info->chat_button = mx_create_chat(mwo->curr_chat, mwo); //change signal connectors
    gtk_list_box_insert(GTK_LIST_BOX(mwo->chatList), mwo->channel_info->chat_button, 0);
    if (mwo->user_ids)
        free(mwo->user_ids);
    mwo->user_ids = NULL;
    g_free(mwo->curr_chat);
    mwo->curr_chat = NULL;
    mx_del_strarr(&mwo->curr_chat_users);
}

void mx_fill_message_info_out(t_mainWindowObjects *mwo, t_json_data *json) {
    t_message_list *temp_mess = NULL;
    t_message_list *temp = NULL;

    mwo->channel_info = mwo->channel_info->first;
    while (mwo->channel_info->next != NULL) {
        if (mwo->channel_info->channel_data.channel_id ==
            json->message.channel_id)
            break;
        mwo->channel_info = mwo->channel_info->next;
    }
    if (mwo->curr_chat_users != NULL && mwo->curr_chat != NULL)
        fill_new_chat(mwo, json);
    temp_mess = malloc(sizeof(t_message_list));
    temp_mess->channel_id = json->message.channel_id;
    temp_mess->message_id = json->message.message_id;
    temp_mess->text = mx_strdup(mwo->curr_message);
    temp_mess->next = NULL;
    if (mwo->channel_info->message == NULL) {
        temp_mess->first = temp_mess;
        mwo->channel_info->message = temp_mess;
        mwo->channel_info->message->mess_row = mx_create_message(mwo->curr_message, mwo, 0); //change signal connectors
        gtk_list_box_insert(GTK_LIST_BOX(mwo->channel_info->messageList), mwo->channel_info->message->mess_row, -1);
    }
    else {
        temp = mwo->channel_info->message;
        mwo->channel_info->message = temp_mess;
        mwo->channel_info->message->next = temp;
        t_message_list *first = mwo->channel_info->message;
        while (mwo->channel_info->message != NULL) {
            mwo->channel_info->message->first = first;
            mwo->channel_info->message = mwo->channel_info->message->next;
        }
        mwo->channel_info->message = first;
        mwo->channel_info->message->mess_row = mx_create_message(mwo->curr_message, mwo, 0); //change signal connectors
        gtk_list_box_insert(GTK_LIST_BOX(mwo->channel_info->messageList), mwo->channel_info->message->mess_row, -1);
    }
    mwo->curr_channel_info = mwo->channel_info;
    mx_strdel(&mwo->curr_message);
    g_timeout_add(200, mx_scroll, mwo);
    gtk_widget_show_all(GTK_WIDGET(mwo->Window));

}

