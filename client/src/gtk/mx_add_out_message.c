#include "uchat_client.h"

static char *strtrim(const char *str) {
    int len = 0;
    char *new = NULL;

    while (*str == '"')
        str++;
    len = mx_strlen(str);
    while (str[len - 1] == '"')
        len--;
    new = mx_strndup(str, len);
    return (char *)new;
}

void mx_add_out_message(t_mainWindowObjects *mwo, t_json_data *json)
{//change :find list by channel_id
    char *temp_str = NULL;
    t_message_list *temp_mess = NULL;
    t_message_list *temp = NULL;
    int flag = 0;
    if (json->message.text == NULL)
        puts("NULL\n");
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
        if (flag == 0) {
            temp_str = strtrim(json->new_channel_data.channel_name);
            strcpy(mwo->channel_info->channel_data.channel_name, temp_str);
            mwo->channel_info->channel_data.channel_id = json->message.channel_id;
            mwo->channel_info->channel_data.last_mes_time = json->new_channel_data.last_mes_time;
            mwo->channel_info->channel_data.user_ids_size = json->new_channel_data.user_ids_size;
            mwo->channel_info->channel_data.user_ids = malloc(sizeof(int) *
                                                              mwo->channel_info->channel_data.user_ids_size);
            for (int i = 0;
                 i < mwo->channel_info->channel_data.user_ids_size; i++) {
                mwo->channel_info->channel_data.user_ids[i] = json->new_channel_data.user_ids[i];
            }
            mwo->channel_info->message = NULL;
            //gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), mwo->curr_messageList);
            mwo->channel_info->messageList = gtk_list_box_new();
            gtk_list_box_set_selection_mode(
                    GTK_LIST_BOX(mwo->channel_info->messageList),
                    GTK_SELECTION_NONE);
//        gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), mwo->curr_messageList);
//        mwo->channel_info->messageList = g_object_ref(mwo->curr_messageList);
//        gtk_container_remove(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), mwo->curr_messageList);
            mwo->channel_info->chat_button = mx_create_chat(
                    mwo->channel_info->channel_data.channel_name,
                    mwo); //change signal connectors
            gtk_list_box_insert(GTK_LIST_BOX(mwo->chatList),
                                mwo->channel_info->chat_button, 0);
            mwo->channel_info->next = malloc(sizeof(t_channel_info));
            //mwo->channel_info->first = mwo->channel_info;
            mwo->channel_info->next->first = mwo->channel_info->first;
            mwo->channel_info->next->chat_button = NULL;
            mwo->channel_info->next->next = NULL;
            mwo->channel_info->next->message = NULL;
            mx_strdel(&temp_str);
        }
        if (mwo->channel_info->message != NULL) {
            temp_mess = malloc(sizeof(t_message_list));
            temp_mess->text = g_strdup_printf("%s", json->message.text);
            temp_mess->channel_id = json->message.channel_id;
            temp_mess->message_id = json->message.message_id;
            temp_mess->delivery_time = json->message.delivery_time;
            temp_mess->next = NULL;
            if (mwo->channel_info->message == NULL) {
                mwo->channel_info->message = temp_mess;
                mwo->channel_info->message->first = mwo->channel_info->message;
                mwo->channel_info->message->mess_row = mx_create_message(
                        mwo->channel_info->message->text, mwo,
                        1); //change signal connectors
                gtk_list_box_insert(
                        GTK_LIST_BOX(mwo->channel_info->messageList),
                        mwo->channel_info->message->mess_row,
                        -1);
            } else {
                temp = mwo->channel_info->message;
                mwo->channel_info->message = temp_mess;
                mwo->channel_info->message->next = temp;
                t_message_list *first = mwo->channel_info->message;
                while (mwo->channel_info->message != NULL) {
                    mwo->channel_info->message->first = first;
                    mwo->channel_info->message = mwo->channel_info->message->next;
                }
                mwo->channel_info->message = first;
                mwo->channel_info->message->mess_row = mx_create_message(
                        mwo->channel_info->message->text, mwo,
                        1); //change signal connectors
                gtk_list_box_insert(
                        GTK_LIST_BOX(mwo->channel_info->messageList),
                        mwo->channel_info->message->mess_row,
                        -1);
            }
        }
        gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    }
}
