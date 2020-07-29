#include "uchat_client.h"

void mx_receiver_mes_del(t_mainWindowObjects *mwo, t_json_data *json) {
    mwo->channel_info = mwo->channel_info->first;
    while (mwo->channel_info->next != NULL) {
        if (mwo->channel_info->channel_data.channel_id ==
            json->message.channel_id)
            break;
        mwo->channel_info = mwo->channel_info->next;
    }
    if (mwo->channel_info->message != NULL) {
        mwo->channel_info->message = mwo->channel_info->message->first;
        while (mwo->channel_info->message->next != NULL) {
            if (mwo->channel_info->message->message_id ==
                json->message.message_id)
                break;
            mwo->channel_info->message = mwo->channel_info->message->next;
        }
        g_object_ref_sink(G_OBJECT(mwo->channel_info->message->mess_row));
        gtk_widget_destroy (mwo->channel_info->message->mess_row);
        g_object_unref (G_OBJECT(mwo->channel_info->message->mess_row));
        gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    }
}

