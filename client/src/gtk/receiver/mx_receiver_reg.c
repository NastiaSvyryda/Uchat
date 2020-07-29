#include "uchat_client.h"

void mx_receiver_reg(t_mainWindowObjects *mwo, t_json_data *json) {
    if (json->status == 200) {
        mwo->user_id = json->user_id;
        strcpy(mwo->login, json->pers_info.login);
        strcpy(mwo->first_name, json->pers_info.first_name);
        strcpy(mwo->last_name, json->pers_info.last_name);
        strcpy(mwo->token, json->token);
        gtk_label_set_text(GTK_LABEL(mwo->label_login),
                           (const gchar *) mwo->login);
        mx_set_component(mwo, mwo->mainWindow);
    }
    if (json->status == 412)
        mx_show_popup(mwo->Window, "Such user already exist!");
}

