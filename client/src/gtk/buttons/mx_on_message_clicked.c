#include "uchat_client.h"

//gboolean mx_enter_edit_textview(__attribute__((unused))GtkWidget *Widget,
//                                GdkEventKey *key, t_mainWindowObjects *mwo) {
//    if (key->keyval == GDK_KEY_Return || key->keyval == GDK_KEY_KP_Enter) {
//        mx_on_edit_message_clicked(NULL, mwo);
//        return TRUE;
//    }
//    return FALSE;
//}

static void init_dialog_edit(t_mainWindowObjects *mwo) {
    GtkBuilder *build = gtk_builder_new();
    GError *error = NULL;
    GtkTextBuffer *buf;

    if (!gtk_builder_add_from_file(build, UI_FILE, &error))
        mx_printstr(error->message);
    mwo->change_mess_Dialog = GTK_WIDGET(
            gtk_builder_get_object(build, "dialog_change_message"));
    gtk_window_set_resizable (GTK_WINDOW (mwo->change_mess_Dialog), FALSE);
    mwo->entry_edit_message = GTK_WIDGET(
            gtk_builder_get_object(build, "edite_message_text_view"));
//    g_signal_connect(mwo->entry_edit_message, "key_press_event",
//            G_CALLBACK(mx_enter_edit_textview), mwo);
    buf = gtk_text_buffer_new (NULL);
    gtk_text_buffer_set_text (buf, mwo->curr_channel_info->message->text,
            (gint)mx_strlen(mwo->curr_channel_info->message->text));
    gtk_text_view_set_buffer ((GtkTextView *)mwo->entry_edit_message, buf);
    g_object_set_data(gtk_builder_get_object(build, "apply_change_message"),
            (gchar *)"message_info", mwo->curr_channel_info->message);
    g_object_set_data(gtk_builder_get_object(build, "delete_message_button"),
            (gchar *)"message_info", mwo->curr_channel_info->message);
    gtk_builder_connect_signals(build, mwo);
    gtk_window_set_transient_for(GTK_WINDOW(mwo->change_mess_Dialog),
                                 mwo->Window);
    gtk_window_set_position(GTK_WINDOW(mwo->change_mess_Dialog),
                            GTK_WIN_POS_CENTER_ON_PARENT);
}

void mx_on_message_clicked(__attribute__((unused)) GtkWidget *button,
                GdkEventButton *event, __attribute__((unused))gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_message_list *curr_message_info = malloc(sizeof(t_message_list));

    if (event->type == GDK_BUTTON_PRESS && event->button == 3 && mwo->edit_mess == 0) {
        curr_message_info = g_object_get_data(G_OBJECT(button),(gchar *)"message_info");
        mwo->curr_channel_info->message = mwo->curr_channel_info->message->first;
        while (mwo->curr_channel_info->message->next != NULL) {
            if (mwo->curr_channel_info->message->message_id ==
                curr_message_info->message_id)
                break;
            mwo->curr_channel_info->message = mwo->curr_channel_info->message->next;
        }
        mwo->edit_mess = 1;
        init_dialog_edit(mwo);
        gtk_widget_show_all(GTK_WIDGET(mwo->change_mess_Dialog));
    }
}
