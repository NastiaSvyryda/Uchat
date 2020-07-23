#include "uchat_client.h"

void mx_error(char *msg)
{
    mx_printstr(msg);
    exit(0);
}

void mx_create_login_window(t_mainWindowObjects *mainObjects) {
    GError *error = NULL;

    mainObjects->builder = gtk_builder_new();
//    gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
//                                               GTK_STYLE_PROVIDER(mainObjects->provider),
//                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    if (!gtk_builder_add_from_file(mainObjects->builder, "sent_gui.glade", &error))
    {
        mx_printstr(error->message);
        mx_error("Cannot add from file!\n");
    }
    g_io_add_watch(g_io_channel_unix_new((gint)mainObjects->fd), G_IO_IN, (GIOFunc)mx_reciever, mainObjects);

    mainObjects->Window = GTK_WINDOW(gtk_builder_get_object(mainObjects->builder, "main_window"));
    mainObjects->loginWindow = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "login_box"));
    mainObjects->mainWindow = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "chats_box"));
    mainObjects->chatWindow = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "chat_box"));
    mainObjects->registreWindow = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "registre_box"));

//    mainObjects->infoDialog = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "dialog_info"));
//    mainObjects->addChat_Dialog = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "dialog_add_chat"));
    mainObjects->entryLogin_l = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "login_entry_l"));
    mainObjects->entryPass_l = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "password_entry_l"));
    mainObjects->entryLogin_r = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "login_entry_r"));
    mainObjects->entryPass_r = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "password_entry_r"));
    mainObjects->entryPass_r2 = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "password_entry_r2"));
    mainObjects->entryName_r = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "name_entry_r"));
    mainObjects->entrySurname_r = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "surname_entry_r"));

    mainObjects->entryChatName = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "entry_chat_name"));
    mainObjects->entryChatUsers = GTK_ENTRY(gtk_builder_get_object(mainObjects->builder, "entry_user_chat"));


    mainObjects->entryMessage = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "message_entry"));

    mainObjects->chatList = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(mainObjects->chatList), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mainObjects->builder, "scrolled_window_chats")), mainObjects->chatList);

    mainObjects->usersList = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(mainObjects->usersList), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mainObjects->builder, "scrolled_window_chatUsers")), mainObjects->usersList);

    mainObjects->label_login = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "label_user"));
    mainObjects->label_chat = GTK_WIDGET(gtk_builder_get_object(mainObjects->builder, "label_curr_chat"));

    gtk_builder_connect_signals(mainObjects->builder, mainObjects);
    gtk_window_set_position(mainObjects->Window, GTK_WIN_POS_CENTER);

    mx_set_component(mainObjects, mainObjects->loginWindow);

    gtk_widget_show_all(GTK_WIDGET(mainObjects->Window));
    gtk_main();

    mx_printstr("after gtk_main\n");
}
