#include "uchat_client.h"

static void push_front_message_list(t_message_list **list, t_json_data *json, int index, t_mainWindowObjects *mwo) {
    t_message_list *front_list = NULL;
    t_message_list *temp = NULL;
    char *temp_mess = NULL;

    front_list = malloc(sizeof(t_message_list));
    front_list->channel_id = json->message.channel_id;
    front_list->message_id = json->messages_arr[index].message_id;
    temp_mess = mx_strtrim_qouts(json->messages_arr[index].text);
    front_list->text = mx_strdup(temp_mess);
    strcpy(front_list->login, json->messages_arr[index].login);
    front_list->next = NULL;
    front_list->first = front_list;
    if (list == NULL)
        mwo->channel_info->message = front_list;
    else {
        temp = *list;
        front_list->next = temp;
        *list = front_list;
        while ((*list)->next != NULL) {
            (*list)->first = front_list;
            *list = (*list)->next;
        }
        (*list)->first = front_list;
        (*list) = front_list;
    }
    mx_strdel(&temp_mess);
}

void mx_receiver_get_history(t_mainWindowObjects *mwo, t_json_data *json) {
    mwo->channel_info = mwo->channel_info->first;
    while (mwo->channel_info->next != NULL) {
        if (mwo->channel_info->channel_data.channel_id ==
            json->message.channel_id)
            break;
        mwo->channel_info = mwo->channel_info->next;
    }
    for (int i = 0; i < json->messages_arr_size; i++) {
        if (mwo->channel_info->message == NULL)
            push_front_message_list(NULL, json, i,mwo);
        else
            push_front_message_list(&mwo->channel_info->message,
                                    json, i, mwo);
        if (json->messages_arr[i].client1_id == mwo->user_id)
            mwo->channel_info->message->mess_row = mx_create_message(
                    mwo->channel_info->message->text, mwo, 0);
        else
            mwo->channel_info->message->mess_row = mx_create_message(
                    mwo->channel_info->message->text, mwo, 1);
        gtk_list_box_insert(
                GTK_LIST_BOX(mwo->channel_info->messageList),
                mwo->channel_info->message->mess_row, 0);
    }
    g_timeout_add(200, mx_scroll, mwo);
    gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    mwo->curr_channel_info = mwo->channel_info;
}

