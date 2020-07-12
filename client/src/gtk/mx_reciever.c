#include "uchat_client.h"

void mx_add_out_message(t_mainWindowObjects *mwo, char *text_char) {
    GtkWidget *row;
    gchar *text;
    if (text_char == NULL)
        puts("NULL\n");
    text = g_strdup_printf("%s", text_char);
    row = mx_create_message(text, mwo);//change signal connectors
    gtk_list_box_insert(GTK_LIST_BOX(mwo->messageList), row, -1);
    gtk_widget_show_all(GTK_WIDGET(mwo->chatWindow));
    free(text);
}

gboolean mx_reciever(__attribute__((unused)) GIOChannel *chan, __attribute__((unused)) GIOCondition condition, void *data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *json_str = NULL;
    int length = 0;
    t_json_data *json = NULL;
    read(mwo->fd, &length, 4);
    json_str = mx_strnew(length);
    read(mwo->fd, json_str, length);
    json = mx_json_parse(json_str);
    mx_printstr("Response recieved:\n");
    mx_printstr(json_str + 4);
    mx_printchar('\n');
    if (json->type == JS_REG) {
        if (json->status == 200) {
            mwo->user_id = json->pers_info.user_id;
            gtk_window_close(mwo->registreWindow);
            mx_create_main_window(mwo);
        }
    }
    else if (json->type == JS_LOG_IN) {
        if (json->status == 200) {
            mwo->user_id = json->pers_info.user_id;
            gtk_window_close(mwo->loginWindow);
            mx_create_main_window(mwo);
        }
    }
    else if (json->type == JS_MES_IN) {
        mx_add_out_message(mwo, (gchar *) json->message.text);
    }
    return TRUE;
}
