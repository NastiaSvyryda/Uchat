#include "uchat_client.h"

gboolean mx_enter_textview(__attribute__((unused))GtkWidget *Widget, GdkEventKey *key, t_mainWindowObjects *mwo) {
    if (key->keyval == GDK_KEY_Return || key->keyval == GDK_KEY_KP_Enter) {
        mx_on_but_send_clicked(NULL, mwo);
        return TRUE;
    }
    return FALSE;
}

static void fill_channel_info(t_mainWindowObjects *mwo, t_json_data *json) {
    if (mwo->curr_chat_users != NULL && mwo->curr_chat != NULL) {
        t_channel channels[1] = {{
                                         .channel_id = -1,
                                         .user_ids_size = mx_arrlen(mwo->curr_chat_users) + 1,
                                         .last_mes_time = 0}};
        strcpy(channels->channel_name, mwo->curr_chat);
        channels->user_ids = malloc(sizeof(int) * channels->user_ids_size);
        for (int i = 0; i < channels->user_ids_size; i++)
            channels->user_ids[i] = mwo->user_ids[i];
        json->message.channel_id = -1;
        json->new_channel_data = channels[0];
        json->new_channel = true;
    }
    else {
        if (mwo->curr_channel_info)
            json->message.channel_id = mwo->curr_channel_info->channel_data.channel_id;
    }
}

void mx_on_but_send_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *tmp = mx_strdup((char *)mx_get_text_of_textview(mwo->entryMessage));
    char *message = mx_handle_user_input(tmp);
    char *json_str = NULL;
    t_json_data *json = NULL;
    if (!mx_validate_message(message, mwo->Window))
        return;

    json = calloc(1, sizeof(t_json_data));
    mwo->curr_message = tmp;
    fill_channel_info(mwo, json);
    json->type = JS_MES_OUT;
    json->message.text = strdup(message);
    json->user_id = mwo->user_id;
    strcpy(json->token, mwo->token);
    json->message.client1_id = mwo->user_id;
    json_str = mx_json_make_json(JS_MES_OUT, json);
    if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
    mx_strdel(&json_str);
    mx_strdel(&json->message.text);
    free(json);
    g_free(message);
}
