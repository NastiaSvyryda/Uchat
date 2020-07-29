#include "uchat_client.h"

static void delete_mess_request(t_mainWindowObjects *mwo) {
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));
    json->message.message_id = mwo->curr_channel_info->message->message_id;
    json->user_id = mwo->user_id;
    json->message.channel_id = mwo->curr_channel_info->channel_data.channel_id;
    strcpy(json->token, mwo->token);
    json_str = mx_json_make_json(JS_MES_DEL_OUT, json);
    if (SSL_connect(mwo->ssl) == -1)
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
    mx_strdel(&json_str);
    free(json);
}

void mx_on_delete_message_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_message_list *curr_message_info = malloc(sizeof(t_message_list));

    curr_message_info = g_object_get_data(G_OBJECT(button),(gchar *)"message_info");
    mwo->curr_channel_info->message = mwo->curr_channel_info->message->first;
    while (mwo->curr_channel_info->message->next != NULL) {
        if (mwo->curr_channel_info->message->message_id ==
            curr_message_info->message_id)
            break;
        mwo->curr_channel_info->message = mwo->curr_channel_info->message->next;
    }
    g_object_ref_sink(G_OBJECT(mwo->curr_channel_info->message->mess_row));
    gtk_widget_destroy (mwo->curr_channel_info->message->mess_row);
    g_object_unref (G_OBJECT(mwo->curr_channel_info->message->mess_row));
    mwo->edit_mess = 0;
    gtk_widget_destroy(mwo->change_mess_Dialog);
    gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    delete_mess_request(mwo);
}
