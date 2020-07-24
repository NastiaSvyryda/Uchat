#include "uchat_client.h"

void mx_on_info_clicked(__attribute__((unused))GtkWidget *button, gpointer data) {//создавать новое диалоговое окно
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    GtkBuilder *builder;
    GError *error = NULL;
    if (mwo->info == 0) {
        mwo->info = 1;
        builder = gtk_builder_new();
    //   gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
    //                                               GTK_STYLE_PROVIDER(mainObjects->provider),
    //                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
        if (!gtk_builder_add_from_file(builder, "sent_gui.glade", &error)) {
            mx_printstr(error->message);
        }
        mwo->infoDialog = GTK_WIDGET(
                 gtk_builder_get_object(builder, "dialog_info"));
        gtk_builder_connect_signals(builder, mwo);
        gtk_window_set_transient_for(GTK_WINDOW(mwo->infoDialog),
                                      mwo->Window);
        gtk_window_set_position(GTK_WINDOW(mwo->infoDialog),
                                 GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_widget_show(mwo->infoDialog);
    }
}

void mx_exit_clicked_info(__attribute__((unused))GtkWidget *button, __attribute__((unused))gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;

    mwo->info = 0;
    gtk_widget_destroy(mwo->infoDialog);
}
