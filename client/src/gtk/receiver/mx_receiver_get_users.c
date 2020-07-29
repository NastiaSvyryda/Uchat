#include "uchat_client.h"

void mx_receiver_get_users(t_mainWindowObjects *mwo, t_json_data *json) {
    GtkWidget *label;
    gchar *text;
    char *temp = NULL;
    int k = 0;

    mwo->ids_logins_arr = malloc(
            sizeof(t_id_login) * (json->ids_logins_arr_size - 1));
    mwo->ids_logins_arr_size = json->ids_logins_arr_size - 1;
    for (int i = 0; i < json->ids_logins_arr_size; i++) {
        if (mwo->user_id != json->ids_logins_arr[i].user_id) {
            temp = mx_strtrim_qouts(json->ids_logins_arr[i].login);
            strcpy(mwo->ids_logins_arr[k].login, temp);
            mwo->ids_logins_arr[k].user_id = json->ids_logins_arr[i].user_id;
            text = g_strdup_printf("%s",
                                   mwo->ids_logins_arr[k].login);
            label = gtk_label_new(text);
            gtk_list_box_insert(GTK_LIST_BOX(mwo->usersList), label,
                                0);
            if (text)
                free(text);
            mx_strdel(&temp);
            k++;
        }
    }
    gtk_widget_show_all(mwo->addChat_Dialog);
}

