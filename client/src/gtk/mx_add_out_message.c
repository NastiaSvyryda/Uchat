#include "uchat_client.h"

void mx_add_out_message(t_mainWindowObjects *mwo, char *text_char)
{//change :find list by channel_id
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
