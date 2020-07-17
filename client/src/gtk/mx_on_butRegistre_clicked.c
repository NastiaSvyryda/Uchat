#include "uchat_client.h"

static void registre_request(t_registre *registre, SSL *ssl)
{
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_REG;
    strcpy(json->pers_info.login, registre->login);
    strcpy(json->pers_info.password, registre->password);
    strcpy(json->pers_info.first_name, registre->name);
    strcpy(json->pers_info.last_name, registre->surname);
    json_str = mx_json_make_json(JS_REG, json);
    if (SSL_connect(ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(ssl, json_str, mx_strlen(json_str + 4) + 4);
}

void mx_on_butRegistre_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_registre registre;

    // gdk_threads_add_idle (mx_input, mwo);
    puts("registre_button");
    registre.type = strdup("register");
    registre.login = (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryLogin_r));
    if (strcmp((char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r)), (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r2))) == 0)
        registre.password = (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r));
    else
        printf("different passwords\n");
    registre.name = (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryName_r));
    registre.surname = (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entrySurname_r));
    if (!mx_valid_string(registre.login) || !mx_valid_string(registre.password) || !mx_valid_string(registre.name) || !mx_valid_string(registre.surname))
    {
        mx_show_popup(mwo->Window, "Each field must consist only of English characters or digits and must be no longer than 255 symbols and have length minimum 3!");
        return;
    }
    registre_request(&registre, mwo->ssl);
    printf("login = %s\npassword = %s\nname = %s\nsurname = %s\n",
           registre.login, registre.password, registre.name,
           registre.surname);
}

void mx_on_butRegistreIn_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mx_set_component(mwo, mwo->registreWindow);
}
