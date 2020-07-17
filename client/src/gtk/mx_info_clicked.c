#include "uchat_client.h"

void mx_info_clicked(__attribute__((unused))GtkWidget *button, gpointer data) {//создавать новое диалоговое окно
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mwo->infoDialog = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "dialog_info"));
    gtk_widget_show(mwo->infoDialog);
    //gtk_dialog_run(GTK_DIALOG(mwo->infoDialog));
}
