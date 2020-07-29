#include "uchat_client.h"

void mx_on_back_login(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mx_set_component(mwo, mwo->loginWindow);
}


void mx_onBack_to_chats(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *)mwo->entryMessage);
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gtk_text_buffer_delete(buffer, &start, &end);
    if (mwo->curr_chat == NULL) {
        g_object_ref(mwo->curr_channel_info->messageList);
        gtk_container_remove(GTK_CONTAINER(mwo->viewPort),
                             mwo->curr_channel_info->messageList);
    }
    mx_set_component(mwo, mwo->mainWindow);
}

static void get_users_request(t_mainWindowObjects *mwo) {
    char *json_str = NULL;
    t_json_data json = {.user_id = mwo->user_id, .type = JS_GET_USERS};

    strcpy(json.token, mwo->token);
    json_str = mx_json_make_json(JS_GET_USERS, &json);
    SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
    mx_strdel(&json_str);
}

void mx_on_but_addchat(__attribute__((unused)) GtkWidget *button, gpointer data) {
    GtkBuilder *builder;
    GError *error = NULL;
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    if (mwo->add_chat == 0) {
        mwo->add_chat = 1;
        get_users_request(mwo);
        builder = gtk_builder_new();
        if (!gtk_builder_add_from_file(builder, UI_FILE, &error))
            mx_printstr(error->message);
        mwo->addChat_Dialog = GTK_WIDGET(
                gtk_builder_get_object(builder, "dialog_add_chat"));
        gtk_window_set_resizable (GTK_WINDOW (mwo->addChat_Dialog), FALSE);
        mwo->entryChatName = GTK_ENTRY(
                gtk_builder_get_object(builder, "entry_chat_name"));
        mwo->usersList = gtk_list_box_new();
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->usersList), GTK_SELECTION_MULTIPLE);
        gtk_builder_connect_signals(builder, mwo);
        gtk_container_add(GTK_CONTAINER(
                gtk_builder_get_object(builder, "scrolled_window_chatUsers")),
                          mwo->usersList);
        gtk_window_set_transient_for(GTK_WINDOW(mwo->addChat_Dialog),
                                     mwo->Window);
        gtk_window_set_position(GTK_WINDOW(mwo->addChat_Dialog),
                                GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_widget_show_all(GTK_WIDGET(mwo->addChat_Dialog));
    }
}
