#include "uchat_client.h"

void mx_apply_add_chat(__attribute__((unused))GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    GtkWidget *row;
    gchar *text;

    gtk_dialog_response (GTK_DIALOG(mwo->addChat_Dialog), GTK_RESPONSE_DELETE_EVENT);
    text = g_strdup_printf("message 2");//считать из entry
    row = mx_create_chat(text, mwo);
    gtk_list_box_insert(GTK_LIST_BOX(mwo->list), row, -1);
    gtk_widget_destroy (mwo->addChat_Dialog);
    gtk_widget_show_all(mwo->mainWindow);
    free(text);
}

void mx_cancel_add_chat(__attribute__((unused))GtkWidget *button, __attribute__((unused))gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;

    gtk_widget_destroy (mwo->addChat_Dialog);
}