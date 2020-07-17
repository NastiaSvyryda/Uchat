#include "uchat_client.h"

void mx_on_message_clicked(__attribute__((unused)) GtkWidget *button, GdkEventButton *event, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    if (event->type == GDK_BUTTON_PRESS && event->button == 3)
    {
        json->type = JS_MES_EDIT_OUT;
        json->message.text = strdup("bye");
        json->message.message_id = 73;
        json->message.client1_id = mwo->user_id;
        strcpy(json->token, mwo->token);
        json_str = mx_json_make_json(JS_MES_EDIT_OUT, json);

        mx_printint(*(int *)json_str);
        if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
            ERR_print_errors_fp(stderr);
        else
            SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
        mx_strdel(&json_str);
        mx_strdel(&json->message.text);
    }
    free(json);
}

GtkWidget *mx_create_message(const gchar *text, struct s_MainWindowObjects *mwo, int align)
{
    GtkWidget *button;

    mwo->mess_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    if (align)
        gtk_widget_set_halign(mwo->mess_row, GTK_ALIGN_START);
    else
        gtk_widget_set_halign(mwo->mess_row, GTK_ALIGN_END);

    button = gtk_button_new_with_label(text);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(mwo->mess_row), button);
    g_signal_connect(button, "button-press-event", G_CALLBACK(mx_on_message_clicked), mwo);
    return mwo->mess_row;
}
