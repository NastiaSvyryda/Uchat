#include "uchat_client.h"

void mx_fill_channel_info_login(t_mainWindowObjects *mwo, t_json_data *json) {
    gchar *text;
    char *temp = NULL;

    for (int i = 0; i < json->channels_arr_size; i++) {//задать первый
        temp = mx_strtrim_qouts(json->channels_arr[i].channel_name);
        strcpy(mwo->channel_info->channel_data.channel_name, temp);
        text = g_strdup_printf("%s", mwo->channel_info->channel_data.channel_name);
        mwo->channel_info->channel_data.channel_id = json->channels_arr[i].channel_id;
        mwo->channel_info->channel_data.last_mes_time = json->channels_arr[i].last_mes_time;
        mwo->channel_info->channel_data.user_ids_size = json->channels_arr[i].user_ids_size;
        mwo->channel_info->channel_data.user_ids = malloc(sizeof(int) * mwo->channel_info->channel_data.user_ids_size);
        for(int j = 0; j < json->channels_arr[i].user_ids_size; j++)
            mwo->channel_info->channel_data.user_ids[j] = json->channels_arr[i].user_ids[j];
        mwo->channel_info->message = NULL;
        mwo->channel_info->messageList = gtk_list_box_new();
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->channel_info->messageList), GTK_SELECTION_NONE);
        mwo->channel_info->chat_button = mx_create_chat(mwo->channel_info->channel_data.channel_name, mwo);
        gtk_list_box_insert(GTK_LIST_BOX(mwo->chatList), mwo->channel_info->chat_button, 0);
        if (text)
            free(text);
        mx_strdel(&temp);
        mwo->channel_info->next = malloc(sizeof(t_channel_info));
        mwo->channel_info->next->first = mwo->channel_info->first;//????
        mwo->channel_info = mwo->channel_info->next;
        mwo->channel_info->next = NULL;
        mwo->channel_info->message = NULL;
    }
    gtk_widget_show_all(mwo->mainWindow);
}

