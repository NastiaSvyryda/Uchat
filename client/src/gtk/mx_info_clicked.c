#include "uchat_client.h"

void mx_info_clicked(__attribute__((unused))GtkWidget *button, gpointer data) {//создавать новое диалоговое окно
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    gtk_widget_show_all(mwo->infoDialog);
    gtk_dialog_run(GTK_DIALOG(mwo->infoDialog));
}
