#include "uchat_client.h"

static void fill_new_channel(t_mainWindowObjects *mwo, t_json_data *json) {
    char *temp_str = NULL;

    temp_str = mx_strtrim_qouts(json->new_channel_data.channel_name);
    strcpy(mwo->channel_info->channel_data.channel_name, temp_str);
    mwo->channel_info->channel_data.channel_id = json->message.channel_id;
    mwo->channel_info->channel_data.last_mes_time = json->new_channel_data.last_mes_time;
    mwo->channel_info->channel_data.user_ids_size = json->new_channel_data.user_ids_size;
    mwo->channel_info->channel_data.user_ids = malloc(sizeof(int) *
                                                      mwo->channel_info->channel_data.user_ids_size);
    for (int i = 0; i < mwo->channel_info->channel_data.user_ids_size; i++)
        mwo->channel_info->channel_data.user_ids[i] = json->new_channel_data.user_ids[i];
    mwo->channel_info->message = NULL;
    mwo->channel_info->messageList = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->channel_info->messageList),
                                    GTK_SELECTION_NONE);
    mwo->channel_info->chat_button = mx_create_chat(
                            mwo->channel_info->channel_data.channel_name, mwo);
    gtk_list_box_insert(GTK_LIST_BOX(mwo->chatList),
                        mwo->channel_info->chat_button, 0);
    mwo->channel_info->next = malloc(sizeof(t_channel_info));
    mwo->channel_info->next->first = mwo->channel_info->first;
    mwo->channel_info->next->chat_button = NULL;
    mwo->channel_info->next->next = NULL;
    mwo->channel_info->next->message = NULL;
    mx_strdel(&temp_str);
}

static void fill_current_mess_info(t_mainWindowObjects *mwo, t_message_list *m) {
    t_message_list *temp = NULL;

    temp = mwo->channel_info->message;
    mwo->channel_info->message = m;
    mwo->channel_info->message->next = temp;
    t_message_list *first = mwo->channel_info->message;
    while (mwo->channel_info->message != NULL) {
        mwo->channel_info->message->first = first;
        mwo->channel_info->message = mwo->channel_info->message->next;
    }
    mwo->channel_info->message = first;
    mwo->channel_info->message->mess_row = mx_create_message(
            mwo->channel_info->message->text, mwo, 1);
    gtk_list_box_insert(GTK_LIST_BOX(mwo->channel_info->messageList),
                        mwo->channel_info->message->mess_row, -1);
}

static void fill_message_info(t_mainWindowObjects *mwo, t_json_data *json) {
    t_message_list *temp_mess = NULL;

    temp_mess = malloc(sizeof(t_message_list));
    temp_mess->text = g_strdup_printf("%s", json->message.text);
    temp_mess->channel_id = json->message.channel_id;
    temp_mess->message_id = json->message.message_id;
    strcpy(temp_mess->login, json->pers_info.login);
    temp_mess->next = NULL;
    if (mwo->channel_info->message == NULL) {
        mwo->channel_info->message = temp_mess;
        mwo->channel_info->message->first = mwo->channel_info->message;
        mwo->channel_info->message->mess_row = mx_create_message(
                mwo->channel_info->message->text, mwo, 1);
        gtk_list_box_insert(GTK_LIST_BOX(mwo->channel_info->messageList),
                            mwo->channel_info->message->mess_row, -1);
    }
    else
        fill_current_mess_info(mwo, temp_mess);
}

void mx_add_out_message(t_mainWindowObjects *mwo, t_json_data *json) {
    int flag = 0;

    mwo->channel_info = mwo->channel_info->first;
    if (mwo->channel_info != NULL) {
        while (mwo->channel_info->next != NULL) {
            if (mwo->channel_info->channel_data.channel_id ==
                json->message.channel_id) {
                flag = 1;
                break;
            }
            mwo->channel_info = mwo->channel_info->next;
        }
        if (flag == 0)
            fill_new_channel(mwo, json);
        if (mwo->channel_info->message != NULL)
            fill_message_info(mwo, json);
        g_timeout_add(200, mx_scroll, mwo);
        gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    }
}
