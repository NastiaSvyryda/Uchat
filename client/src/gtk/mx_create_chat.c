#include "uchat_client.h"

void mx_on_chat_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    //gtk_label_set_text(GTK_LABEL(mwo->label_chat),);
    mx_set_component(mwo, mwo->chatWindow);
}

GtkWidget *mx_create_chat(const gchar *text, struct s_MainWindowObjects *mwo)
{
    GtkWidget *button;

    mwo->chat_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    button = gtk_button_new_with_label(text);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(mwo->chat_row), button);
    g_signal_connect(button, "clicked", G_CALLBACK(mx_on_chat_clicked), mwo);
    return mwo->chat_row;
}
