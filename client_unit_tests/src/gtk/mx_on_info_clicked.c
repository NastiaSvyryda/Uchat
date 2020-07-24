#include "uchat_client.h"

void mx_on_info_clicked(__attribute__((unused))GtkWidget *button, gpointer data) {//создавать новое диалоговое окно
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mwo->infoDialog = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "dialog_info"));
    gtk_widget_show(mwo->infoDialog);
    gtk_main();
}

void mx_exit_clicked_info(__attribute__((unused))GtkWidget *button, __attribute__((unused))gpointer data) {
    //t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;

//    gtk_widget_hide(mwo->infoDialog);
//    gtk_main_quit();
//    gtk_widget_destroy (mwo->infoDialog);
}
