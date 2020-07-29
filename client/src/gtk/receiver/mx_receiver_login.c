#include "uchat_client.h"

void mx_receiver_login(t_mainWindowObjects *mwo, t_json_data *json) {
    if (json->status == 200) {
        mwo->user_id = json->user_id;
        strcpy(mwo->login, json->pers_info.login);
        strcpy(mwo->first_name, json->pers_info.first_name);
        strcpy(mwo->last_name, json->pers_info.last_name);
        strcpy(mwo->token, json->token);
        gtk_label_set_text(GTK_LABEL(mwo->label_login),
                           (const gchar *) mwo->login);//change label
        mx_fill_channel_info_login(mwo, json);
        mx_set_component(mwo, mwo->mainWindow);
    }
    if (json->status == 401)
        mx_show_popup(mwo->Window, "Invalid user or password!");
    if (json->status == 403)
        mx_show_popup(mwo->Window, "This user is already signed in!");
}

