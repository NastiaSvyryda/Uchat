#include "uchat_client.h"

void mx_show_popup(void *parent_window, char *msg)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_CLOSE, "%s",
                                               msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

_Bool mx_valid_string(char *str)
{
    if (!str || !(*str) || strlen(str) < 3)
        return 0;
    while (*str)
    {
        if (!(isalpha(*str) || isdigit(*str)))
            return 0;
        str++;
    }
    return 1;
}

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
    login.login = (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryLogin_l));
    login.password = (char *)gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_l));
    if (!mx_valid_string(login.login))
    {
        mx_show_popup(mwo->Window, "Invalid login! Login must consist only of English characters or digits and must be no longer than 255 symbols and have length minimum 3!");
        return;
    }
    if (!mx_valid_string(login.password))
    {
        mx_show_popup(mwo->Window, "Invalid password! Password must consist only of English characters or digits and must be no longer than 255 symbols and have length minimum 3!");
        return;
    }
    login_request(&login, mwo->ssl);
    printf("login = %s\npassword = %s\n", login.login, login.password);
}
