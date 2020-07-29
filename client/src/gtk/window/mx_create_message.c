#include "uchat_client.h"

static void set_message_align(t_mainWindowObjects *mwo, int align,
                                GtkWidget *button, GtkWidget *row) {
    GtkWidget *label_login;
    char *tmp = NULL;

    if (align) {
        tmp = mx_strtrim_qouts(mwo->channel_info->message->login);
        label_login = gtk_label_new(tmp);//
        gtk_container_add(GTK_CONTAINER(row), label_login);
        gtk_widget_set_halign(label_login, GTK_ALIGN_START);
        gtk_widget_set_halign(row, GTK_ALIGN_START);
        gtk_widget_set_name (button, "left_button");
        gtk_widget_set_name (label_login, "left_label");
        mx_strdel(&tmp);
    }
    else {
        gtk_widget_set_halign(row, GTK_ALIGN_END);
        g_object_set_data(G_OBJECT(button), (gchar *)"message_info", mwo->channel_info->message);
        g_signal_connect(button, "button-press-event", G_CALLBACK(mx_on_message_clicked), mwo);
        gtk_widget_set_name (button, "right_button");
    }
}
GtkWidget *mx_create_message(const gchar *text, t_mainWindowObjects *mwo, int align) {
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *row;

    row = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    button = gtk_button_new();
    label = gtk_label_new(text);
    gtk_label_set_line_wrap((GtkLabel *)label, TRUE);
    gtk_label_set_max_width_chars (GTK_LABEL (label), 40);
    gtk_label_set_line_wrap_mode((GtkLabel *)label, PANGO_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(button), label);
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    set_message_align(mwo, align, button, row);
    gtk_container_add(GTK_CONTAINER(row), button);
    return row;
}
