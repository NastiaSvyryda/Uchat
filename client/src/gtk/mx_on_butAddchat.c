#include "uchat_client.h"

void mx_onBack_to_chats(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    gtk_widget_hide(mwo->channel_info->messageList);
    mx_set_component(mwo, mwo->mainWindow);
}

void mx_on_butAddchat(__attribute__((unused)) GtkWidget *button, gpointer data)
{ //создавать новое диалоговое окно
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *json_str = NULL;
    t_json_data json = {.user_id = mwo->user_id, .type = JS_GET_USERS};
    strcpy(json.token, mwo->token);
    json_str = mx_json_make_json(JS_GET_USERS, &json);
    mx_printstr(json_str + 4);
    SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
    mx_strdel(&json_str);

    mwo->addChat_Dialog = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "dialog_add_chat"));
    mwo->entryChatName = GTK_ENTRY(gtk_builder_get_object(mwo->builder, "entry_chat_name"));
    //обработать ответ от сервера с логинами в reciever
    gtk_widget_show_all(GTK_WIDGET(mwo->addChat_Dialog));
    //gtk_dialog_run(GTK_DIALOG(mwo->addChat_Dialog));
}
