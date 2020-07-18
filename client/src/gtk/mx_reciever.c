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

static void fill_channel_info(t_mainWindowObjects *mwo, t_json_data *json) {
    gchar *text;
    char *temp = NULL;

    for (int i = 0; i < json->channels_arr_size; i++) {
        temp = strtrim(json->channels_arr[i].channel_name);
        strcpy(mwo->channel_info->channel_data.channel_name, temp);
        text = g_strdup_printf("%s", mwo->channel_info->channel_data.channel_name);
        mwo->channel_info->chat_button = mx_create_chat(mwo->channel_info->channel_data.channel_name, mwo);
        gtk_list_box_insert(GTK_LIST_BOX(mwo->chatList), mwo->channel_info->chat_button, 0);
        mwo->channel_info->channel_data.channel_id = json->channels_arr[i].channel_id;
        mwo->channel_info->channel_data.last_mes_time = json->channels_arr[i].last_mes_time;
        mwo->channel_info->channel_data.user_ids_size = json->channels_arr[i].user_ids_size;
        mwo->channel_info->channel_data.user_ids = malloc(sizeof(int) * mwo->channel_info->channel_data.user_ids_size);
        for(int j = 0; j < json->channels_arr[i].user_ids_size; j++) {
            mwo->channel_info->channel_data.user_ids[j] = json->channels_arr[i].user_ids[j];
        }
        mwo->channel_info->message = malloc(sizeof(t_message_list));
        mwo->channel_info->message->first = NULL;
        mwo->channel_info->message->next = NULL;
        mwo->channel_info->next = malloc(sizeof(t_channel_info));
        mwo->channel_info->next->first = mwo->channel_info->first;
        mwo->channel_info = mwo->channel_info->next;
        free(text);
        mx_strdel(&temp);
    }
    gtk_widget_show_all(mwo->mainWindow);
}

static void fill_message_info_(t_mainWindowObjects *mwo, t_json_data *json) {
    mwo->channel_info = mwo->channel_info->first;
    while (mwo->channel_info->channel_data.channel_id != json->message.channel_id) {
        mwo->channel_info = mwo->channel_info->next;
    }
    if (mwo->channel_info->message->first == NULL) {
        mwo->channel_info->message->first = mwo->channel_info->message;
    }
//    mwo->channel_info->message->next = NULL;
//    while (mwo->channel_info->message->next != NULL)
//        mwo->channel_info->message = mwo->channel_info->message->next;
    mwo->channel_info->message->message_id = json->message.message_id;
    mwo->channel_info->message->channel_id = json->message.channel_id;
    mwo->channel_info->message->delivery_time = json->message.delivery_time;
    mwo->channel_info->message->next = malloc(sizeof(t_message_list));
    mwo->channel_info->message->next->first = mwo->channel_info->message->first;
    mwo->channel_info->message = mwo->channel_info->message->next;
}

gboolean mx_reciever(__attribute__((unused)) GIOChannel *chan, __attribute__((unused)) GIOCondition condition, void *data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *json_str = NULL;
    int length = 0;
    t_json_data *json = NULL;
    SSL_read(mwo->ssl, &length, 4);
    json_str = mx_strnew(length);
    SSL_read(mwo->ssl, json_str, length);
    json = mx_json_parse(json_str);
    mx_printstr("Response recieved:\n");
    mx_printstr(json_str);
    mx_printchar('\n');
    if (json->type == JS_REG)
    {
        if (json->status == 200)
        {
            mwo->user_id = json->user_id;
            strcpy(mwo->login, json->pers_info.login);
            strcpy(mwo->first_name, json->pers_info.first_name);
            strcpy(mwo->last_name, json->pers_info.last_name);
            strcpy(mwo->token, json->token);
            gtk_label_set_text(GTK_LABEL(mwo->label_login), (const gchar *)mwo->login);
            mx_set_component(mwo, mwo->mainWindow);
        }
    }
    else if (json->type == JS_LOG_IN)
    {
        if (json->status == 200)
        {
            fill_channel_info(mwo, json);
            mwo->user_id = json->user_id;
            strcpy(mwo->login, json->pers_info.login);
            strcpy(mwo->first_name, json->pers_info.first_name);
            strcpy(mwo->last_name, json->pers_info.last_name);
            strcpy(mwo->token, json->token);
            gtk_label_set_text(GTK_LABEL(mwo->label_login), (const gchar *)mwo->login);
            mx_set_component(mwo, mwo->mainWindow);
        }
    }
    else if (json->type == JS_MES_IN)
    {
        mx_add_out_message(mwo, json->message.text);
    }
    else if (json->type == JS_MES_OUT)
    {
        fill_message_info_(mwo, json);
    }
    else if (json->type == JS_GET_USERS)//для добавления юзеров
    {
        GtkWidget *row;
        GtkWidget *label;
        gchar *text;
        char *temp = NULL;
        mwo->ids_logins_arr = malloc(sizeof(t_id_login) * json->ids_logins_arr_size);
        for (int i = 0; i < json->ids_logins_arr_size; i++) {
            temp = strtrim(json->ids_logins_arr[i].login);
            strcpy(mwo->ids_logins_arr[i].login, temp);
            mwo->ids_logins_arr->user_id = json->ids_logins_arr[i].user_id;
            text = g_strdup_printf("%s", mwo->ids_logins_arr[i].login);
            row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
            label = gtk_label_new (text);
            gtk_container_add(GTK_CONTAINER(row), label);
            gtk_list_box_insert(GTK_LIST_BOX(mwo->usersList), row, 0);
            free(text);
            mx_strdel(&temp);
        }
        gtk_widget_show_all(mwo->addChat_Dialog);
    }
    else if (json->type == JS_MES_HIST) {
//        mwo->channel_info = mwo->channel_info->first;
//        while (mwo->channel_info->channel_data.channel_id != json->message.channel_id) {
//            mwo->channel_info = mwo->channel_info->next;
//        }
    }
    return TRUE;
}
