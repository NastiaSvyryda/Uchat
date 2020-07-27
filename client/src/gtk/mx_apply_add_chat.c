#include "uchat_client.h"

void mx_apply_add_chat(__attribute__((unused)) GtkWidget *button, gpointer data) { //открывать переписку а уже при оправке первого смс создать чат
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    int len = 0;
    int j = 0;

    if (mwo->ids_logins_arr) {
        mwo->add_chat = 0;
        mwo->curr_chat = (gchar *)gtk_entry_get_text(mwo->entryChatName);
        if (!mx_validate_chat_name(mwo->curr_chat, GTK_WINDOW(mwo->addChat_Dialog)))
            gtk_widget_destroy(mwo->addChat_Dialog);
        mwo->curr_chat_users = mx_strsplit(
                gtk_entry_get_text(mwo->entryChatUsers), ' ');//VALIDATION!
        len = mx_arrlen(mwo->curr_chat_users);
        mwo->user_ids = malloc(sizeof(int) * (len + 1));
        for (int i = 0; i < len; i++) {
            j = 0;
            while (j < mwo->ids_logins_arr_size) {
                if (strcmp(mwo->curr_chat_users[i],
                           mwo->ids_logins_arr[j].login) == 0)
                    mwo->user_ids[i] = mwo->ids_logins_arr[j].user_id;
                puts(mwo->ids_logins_arr[j].login);
                puts("\n");
                mx_printint(mwo->ids_logins_arr[j].user_id);
                puts("\n");
                j++;
            }
        }
        mwo->user_ids[len] = mwo->user_id;
        if (!mx_validate_chat_name(mwo->curr_chat, mwo->Window))
            return;
        //gtk_widget_destroy(mwo->addChat_Dialog);
        if (mwo->ids_logins_arr) {
            free(mwo->ids_logins_arr);
            mwo->ids_logins_arr = NULL;
        }
        mx_set_component(mwo, mwo->chatWindow);
        gtk_widget_destroy(mwo->addChat_Dialog);
    }
}

void mx_cancel_add_chat(__attribute__((unused)) GtkWidget *button, __attribute__((unused)) gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    //g_clear_object(&mwo->addChat_Dialog);
    //g_clear_object(&mwo->entryChatName);
    //g_clear_object(&mwo->usersList);
//    if (mwo->ids_logins_arr)
//        free(mwo->ids_logins_arr);
    mwo->add_chat = 0;
    gtk_widget_destroy(mwo->addChat_Dialog);
}
