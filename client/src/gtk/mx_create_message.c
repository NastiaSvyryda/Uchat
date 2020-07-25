#include "uchat_client.h"

void mx_on_edit_message_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {//delete child and replace button
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_message_list *curr_message_info = malloc(sizeof(t_message_list));

    curr_message_info = g_object_get_data(G_OBJECT(button),(gchar *)"message_info");
    mwo->channel_info->message = mwo->channel_info->message->first;
    //if (mwo->channel_info->message) {
        while (mwo->channel_info->message != NULL) {
            if (mwo->channel_info->message->message_id ==
                curr_message_info->message_id)
                break;
            mwo->channel_info->message = mwo->channel_info->message->next;
        }
    //}

}

void mx_on_delete_message_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_message_list *curr_message_info = malloc(sizeof(t_message_list));

    curr_message_info = g_object_get_data(G_OBJECT(button),(gchar *)"message_info");
    mwo->channel_info->message = mwo->channel_info->message->first;
    //if (mwo->channel_info->message) {
    while (mwo->channel_info->message != NULL) {
        if (mwo->channel_info->message->message_id ==
            curr_message_info->message_id)
            break;
        mwo->channel_info->message = mwo->channel_info->message->next;
    }
    //}

}

void mx_on_message_clicked(__attribute__((unused)) GtkWidget *button, GdkEventButton *event, __attribute__((unused))gpointer data) {
    //t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    //char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    if (event->type == GDK_BUTTON_PRESS && event->button == 3) {

//        json->type = JS_MES_EDIT_OUT;
//        json->message.text = strdup("bye");
//        json->message.message_id = 73;
//        json->message.client1_id = mwo->user_id;
//        strcpy(json->token, mwo->token);
//        json_str = mx_json_make_json(JS_MES_EDIT_OUT, json);
//
//        //mx_printint(*(int *)json_str + 4);
//        if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
//            ERR_print_errors_fp(stderr);
//        else
//            SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
//        mx_strdel(&json_str);
//        mx_strdel(&json->message.text);
    }
    free(json);
}

GtkWidget *mx_create_message(const gchar *text, struct s_MainWindowObjects *mwo, int align) {
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *row;

    row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    button = gtk_button_new();
    label = gtk_label_new(text);
    gtk_label_set_line_wrap((GtkLabel *)label, TRUE);
    gtk_label_set_line_wrap_mode((GtkLabel *)label, PANGO_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(button), label);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(row), button);
    if (align)
        gtk_widget_set_halign(row, GTK_ALIGN_START);
    else {
        gtk_widget_set_halign(row, GTK_ALIGN_END);
        g_signal_connect(button, "button-press-event", G_CALLBACK(mx_on_message_clicked), mwo);
        g_object_set_data(G_OBJECT(button), (gchar *)"message_info", mwo->channel_info->message);
    }
    return row;
}
