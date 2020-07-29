#include "uchat_client.h"

static void mess_history_request(t_mainWindowObjects *mwo) {
    char *json_str = NULL;
    t_json_data json = {.user_id = mwo->user_id, .message.last_message_id = -1, .type = JS_MES_HIST};
    strcpy(json.token, mwo->token);
    json.message.channel_id = mwo->channel_info->channel_data.channel_id;
    json_str = mx_json_make_json(JS_MES_HIST, &json);
    if (SSL_connect(mwo->ssl) == -1)
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
}

void mx_on_chat_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    int *channel_id = (int *)g_object_get_data(G_OBJECT(button),(gchar *)"channel_id");

    mwo->channel_info = mwo->channel_info->first;
    if (mwo->channel_info) {
        while (mwo->channel_info->next != NULL) {
            if (mwo->channel_info->channel_data.channel_id ==
                *channel_id)
                break;
            mwo->channel_info = mwo->channel_info->next;
        }
    }
    mwo->curr_channel_info = mwo->channel_info;
    if (mwo->channel_info->message == NULL) {
        mess_history_request(mwo);
    }
    gtk_label_set_text(GTK_LABEL(mwo->label_chat),(const gchar *)mwo->channel_info->channel_data.channel_name);
    gtk_container_add(GTK_CONTAINER(mwo->viewPort), mwo->channel_info->messageList);
    mx_set_component(mwo, mwo->chatWindow);
    g_timeout_add(200, mx_scroll, mwo);
    gtk_widget_show_all(GTK_WIDGET(mwo->Window));
}
