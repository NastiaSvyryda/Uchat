#include "uchat_client.h"

void mx_add_out_message(t_mainWindowObjects *mwo, char *text_char)
{
    GtkWidget *row;
    gchar *text;
    if (text_char == NULL)
        puts("NULL\n");
    text = g_strdup_printf("%s", text_char);
    row = mx_create_message(text, mwo, 1); //change signal connectors
    gtk_list_box_insert(GTK_LIST_BOX(mwo->messageList), row, -1);
    gtk_widget_show_all(mwo->chatWindow);
    free(text);
}

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
        mx_printstr("\nname:");

        temp = strtrim(json->channels_arr[i].channel_name);
        strcpy(mwo->channel_info->channel_data.channel_name, temp);
        text = g_strdup_printf("%s", mwo->channel_info->channel_data.channel_name);
        mwo->channel_info->chat_button = mx_create_chat(mwo->channel_info->channel_data.channel_name, mwo);
        gtk_list_box_insert(GTK_LIST_BOX(mwo->list), mwo->channel_info->chat_button, 0);
        mwo->channel_info->channel_data.channel_id = json->channels_arr[i].channel_id;
        mwo->channel_info->channel_data.last_mes_time = json->channels_arr[i].last_mes_time;
        mwo->channel_info->channel_data.user_ids_size = json->channels_arr[i].user_ids_size;
        mwo->channel_info->channel_data.user_ids = malloc(sizeof(int) * mwo->channel_info->channel_data.user_ids_size);
        for(int j = 0; j < json->channels_arr[i].user_ids_size; j++) {
            mwo->channel_info->channel_data.user_ids[j] = json->channels_arr[i].user_ids[j];
        }
        mwo->channel_info->next = malloc(sizeof(t_channel_info));
        mwo->channel_info->next->first = mwo->channel_info->first;
        mwo->channel_info = mwo->channel_info->next;
        free(text);
        mx_strdel(&temp);
    }
    gtk_widget_show_all(mwo->mainWindow);
}

gboolean mx_reciever(__attribute__((unused)) GIOChannel *chan, __attribute__((unused)) GIOCondition condition, void *data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *json_str = NULL;
    int length = 0;
    t_json_data *json = NULL;
    SSL_read(mwo->ssl, &length, 4);
    //    length -= 4; //влада джсон надо перепроверить, поведение с длиной строки
    json_str = mx_strnew(length);
    SSL_read(mwo->ssl, json_str, length);
    json = mx_json_parse(json_str);
    mx_printstr("Response recieved:\n");
    mx_printstr(json_str + 4);
    mx_printchar('\n');
    if (json->type == JS_REG)
    {
        if (json->status == 200)
        {
            mwo->user_id = json->user_id;
            strcpy(mwo->token, json->token);

            mx_set_component(mwo, mwo->mainWindow);
        }
    }
    else if (json->type == JS_LOG_IN)
    {
        if (json->status == 200)
        {
            fill_channel_info(mwo, json);
            mwo->user_id = json->user_id;
            strcpy(mwo->token, json->token);
            mx_set_component(mwo, mwo->mainWindow);
        }
    }
    else if (json->type == JS_MES_IN)
    {
        mx_add_out_message(mwo, json->message.text);
    }
    return TRUE;
}
