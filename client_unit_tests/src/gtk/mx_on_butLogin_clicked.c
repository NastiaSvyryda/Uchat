#include "uchat_client.h"

static void login_request(t_login *login, SSL *ssl)
{
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    strcpy(json->pers_info.login, login->login);
    strcpy(json->pers_info.password, login->password);
    json->type = JS_LOG_IN;
    json->user_id = 55555;
    strcpy(json->token, " ");
    json_str = mx_json_make_json(JS_LOG_IN, json);

    if (SSL_connect(ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(ssl, json_str, mx_strlen(json_str + 4) + 4);
}

void mx_on_butLogin_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_login login;

    login.type = strdup("login");
    login.login = mx_handle_user_input((char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryLogin_l)));
    login.password = mx_handle_user_input((char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_l)));

    _Bool valid = mx_validate_login(login.login, mwo->Window) && mx_validate_password(login.password, mwo->Window);
    if (!valid)
        return;

    login_request(&login, mwo->ssl);
    printf("login = %s\npassword = %s\n", login.login, login.password);
}
