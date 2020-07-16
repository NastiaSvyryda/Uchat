#include "uchat_client.h"

void mx_exit_clicked_info(__attribute__((unused))GtkWidget *button, __attribute__((unused))gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
//
//    gtk_widget_hide(mwo->infoDialog);
    //gtk_main_quit();
    gtk_widget_destroy (mwo->infoDialog);
}
