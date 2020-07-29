#include "uchat_client.h"

static void append_login_to_str(gpointer mem, gpointer all) {
    char **tt = (char**)all;
    GtkLabel *u = (GtkLabel *)gtk_bin_get_child((GtkBin *)mem);
    const gchar *login = gtk_label_get_text ((GtkLabel *)u);

    if (strlen(*tt) != 0)
        *tt = mx_strjoin(*tt, " ");
    *tt = mx_strjoin(*tt, login);
}

static void fill_new_chat_info(t_mainWindowObjects *mwo, char *all) {
    int j = 0;
    int len = 0;

    mwo->curr_chat_users = mx_strsplit(all,' ');
    len = mx_arrlen(mwo->curr_chat_users);
    mwo->user_ids = malloc(sizeof(int) * (len + 1));
    for (int i = 0; i < len; i++) {
        j = 0;
        while (j < mwo->ids_logins_arr_size) {
            if (strcmp(mwo->curr_chat_users[i],
                       mwo->ids_logins_arr[j].login) == 0)
                mwo->user_ids[i] = mwo->ids_logins_arr[j].user_id;
            j++;
        }
    }
    mwo->user_ids[len] = mwo->user_id;
    if (mwo->ids_logins_arr) {
        free(mwo->ids_logins_arr);
        mwo->ids_logins_arr = NULL;
    }
}

void mx_apply_add_chat(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    char *all = NULL;
    GList *selected_users;

    if (mwo->ids_logins_arr) {
        mwo->add_chat = 0;
        mwo->curr_chat = mx_handle_user_input((char *)gtk_entry_get_text(mwo->entryChatName));
        if (mx_validate_chat_name(mwo->curr_chat, GTK_WINDOW(mwo->addChat_Dialog))) {
            all = strdup("");
            selected_users = gtk_list_box_get_selected_rows ((GtkListBox *)mwo->usersList);
            g_list_foreach(selected_users, (GFunc)append_login_to_str, &all);
            if (strlen(all) > 0) {
                fill_new_chat_info(mwo, all);
                gtk_label_set_text(GTK_LABEL(mwo->label_chat),
                                   (const gchar *) mwo->curr_chat);
                mx_set_component(mwo, mwo->chatWindow);
                gtk_widget_destroy(mwo->addChat_Dialog);
                mx_strdel(&all);
            }
            else
                mx_show_popup(mwo->Window, "You should choose at least 1 member to create chat!");
        }
    }
}

void mx_cancel_add_chat(__attribute__((unused)) GtkWidget *button,
                        __attribute__((unused)) gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mwo->add_chat = 0;
    gtk_widget_destroy(mwo->addChat_Dialog);
}
