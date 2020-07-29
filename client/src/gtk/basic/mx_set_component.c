#include "uchat_client.h"

void mx_set_component(t_mainWindowObjects *mwo, GtkWidget *gtk_component) {
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "gtkbox")));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_container_remove(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "gtkbox")), GTK_WIDGET(iter->data));
    }
    gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(mwo->builder, "gtkbox")), gtk_component, TRUE, TRUE, 5);
    gtk_widget_show_all(GTK_WIDGET(mwo->Window));
    // g_list_free(children);
}
