#include "uchat_client.h"

void mx_apply_add_chat(__attribute__((unused)) GtkWidget *button, gpointer data) { //открывать переписку а уже при оправке первого смс создать чат
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    GtkWidget *row;
    int i = 0;
    int len = 0;
    int j = 0;

    //gtk_dialog_response(GTK_DIALOG(mwo->addChat_Dialog), GTK_RESPONSE_DELETE_EVENT);
    mwo->curr_chat = (gchar *) mx_handle_user_input(gtk_entry_get_text(mwo->entryChatName));
    mwo->curr_chat_users = mx_strsplit(gtk_entry_get_text(mwo->entryChatUsers), ' ');
    len = mx_arrlen(&mwo->curr_chat);
    mwo->user_ids = malloc(sizeof(int) * len);
    for (int i = 0; i < len; i++) {
        j = 0;
        while (strcmp(mwo->curr_chat_users[i], mwo->ids_logins_arr[j].login) != 0 && j < mwo->ids_logins_arr_size) {
            j++;
        }
        mwo->user_ids[i] = mwo->ids_logins_arr[j].user_id;
    }
    while (mwo->curr_chat_users[i]) {
        puts(mwo->curr_chat_users[i++]);
        puts("\n");
    }
    if (!mx_validate_chat_name(mwo->curr_chat, mwo->Window))
        return;
    row = mx_create_chat(mwo->curr_chat, mwo);
    gtk_list_box_insert(GTK_LIST_BOX(mwo->chatList), row, -1);
    //gtk_widget_destroy(mwo->addChat_Dialog);
    mx_set_component(mwo, mwo->chatWindow);
}

void mx_cancel_add_chat(__attribute__((unused)) GtkWidget *button, __attribute__((unused)) gpointer data) {
    //t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    gtk_main_quit();
}
