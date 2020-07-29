#include "uchat_client.h"

void mx_error(char *msg) {
    mx_printstr(msg);
    exit(0);
}

static void init_gtk(t_mainWindowObjects *mwo) {
    mwo->viewPort = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "scrolled_chat"));
    mwo->Window = GTK_WINDOW(gtk_builder_get_object(mwo->builder, "main_window"));
    mwo->loginWindow = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "login_box"));
    mwo->mainWindow = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "chats_box"));
    mwo->chatWindow = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "chat_box"));
    mwo->registreWindow = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "registre_box"));
    mwo->entryLogin_l = GTK_ENTRY(gtk_builder_get_object(mwo->builder, "login_entry_l"));
    mwo->entryPass_l = GTK_ENTRY(gtk_builder_get_object(mwo->builder, "password_entry_l"));
    mwo->entryLogin_r = GTK_ENTRY(gtk_builder_get_object(mwo->builder, "login_entry_r"));
    mwo->entryPass_r = GTK_ENTRY(gtk_builder_get_object(mwo->builder, "password_entry_r"));
    mwo->entryPass_r2 = GTK_ENTRY(gtk_builder_get_object(mwo->builder, "password_entry_r2"));
    mwo->entryName_r = GTK_ENTRY(gtk_builder_get_object(mwo->builder, "name_entry_r"));
    mwo->entrySurname_r = GTK_ENTRY(gtk_builder_get_object(mwo->builder, "surname_entry_r"));
    mwo->entryMessage = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "message_entry"));
    mwo->label_login = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "label_user"));
    mwo->label_chat = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "label_curr_chat"));
    mwo->chatList = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->chatList), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_window_chats")), mwo->chatList);
    g_signal_connect(mwo->entryMessage, "key_press_event", G_CALLBACK(mx_enter_textview), mwo);
}

void mx_create_login_window(t_mainWindowObjects *mwo) {
    GError *error = NULL;

    mwo->builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(mwo->builder, UI_FILE, &error)) {
        mx_printstr(error->message);
        mx_error("Cannot add from file!\n");
    }
    g_io_add_watch(g_io_channel_unix_new((gint)mwo->fd), G_IO_IN, (GIOFunc)mx_receiver, mwo);
    init_gtk(mwo);
    gtk_window_set_resizable (GTK_WINDOW (mwo->Window), FALSE);
    gtk_builder_connect_signals(mwo->builder, mwo);
    gtk_window_set_position(mwo->Window, GTK_WIN_POS_CENTER);
    mx_set_component(mwo, mwo->loginWindow);
    gtk_main();
    //mx_printstr("after gtk_main\n");
}
