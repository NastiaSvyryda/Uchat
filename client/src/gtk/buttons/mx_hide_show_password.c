#include "uchat_client.h"

void mx_hide_show_password(__attribute__((unused))GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    if (mwo->visibility == 1) {
        gtk_entry_set_visibility(GTK_ENTRY(mwo->entryPass_l), TRUE);
        mwo->visibility = 0;
    }
    else {
        gtk_entry_set_visibility(GTK_ENTRY(mwo->entryPass_l), FALSE);
        mwo->visibility = 1;
    }
}
