#include "uchat_client.h"

GtkWidget *mx_create_chat(const gchar *text, struct s_MainWindowObjects *mwo) {
    GtkWidget *button;
    GtkWidget *chat_row;

    chat_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    button = gtk_button_new_with_label(text);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(chat_row), button);
    g_signal_connect(button, "clicked", G_CALLBACK(mx_on_chat_clicked), mwo);
    g_object_set_data(G_OBJECT(button), (gchar *)"channel_id",&mwo->channel_info->channel_data.channel_id);
    return chat_row;
}
