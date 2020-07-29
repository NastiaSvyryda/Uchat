#include "uchat_client.h"

gint mx_scroll(gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    GtkWidget *scrolled_window = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "chatScrolledWindow"));
    GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
    double value = gtk_adjustment_get_upper(adj);

    gtk_adjustment_set_value(adj, value);
    return false;
}
