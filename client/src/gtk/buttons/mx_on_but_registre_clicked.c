#include "uchat_client.h"

static void registre_request(t_registre *registre, SSL *ssl) {
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_REG;
    strcpy(json->pers_info.login, registre->login);
    strcpy(json->pers_info.password, registre->password);
    strcpy(json->pers_info.first_name, registre->name);
    strcpy(json->pers_info.last_name, registre->surname);
    json_str = mx_json_make_json(JS_REG, json);
    if (SSL_connect(ssl) == -1)
        ERR_print_errors_fp(stderr);
    else
        SSL_write(ssl, json_str, *(int *)json_str + 4);
}

void mx_on_but_registre_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_registre registre;
    _Bool valid;

    registre.type = strdup("register");
    registre.login = mx_handle_user_input((char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryLogin_r)));
    if (strcmp((char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r)), (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r2))) == 0)
        registre.password = mx_handle_user_input((char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r)));
    else{
        mx_show_popup(mwo->Window, "Different passwords!");
        return;
    }
    registre.name = mx_handle_user_input((char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryName_r)));
    registre.surname = mx_handle_user_input((char *)gtk_entry_get_text(GTK_ENTRY(mwo->entrySurname_r)));
    valid = mx_validate_login(registre.login, mwo->Window) &&
                  mx_validate_password(registre.password, mwo->Window) &&
                  mx_validate_user_name(registre.name, mwo->Window) &&
                  mx_validate_user_surname(registre.surname, mwo->Window);
    if (!valid)
        return;
    registre_request(&registre, mwo->ssl);
}

void mx_on_butRegistreIn_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mx_set_component(mwo, mwo->registreWindow);
}
