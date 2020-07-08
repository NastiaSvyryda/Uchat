#include "uchat_client.h"

static void login_request(t_login *login, int fd) {
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    strcpy(json->pers_info.login, login->login);
    strcpy(json->pers_info.password, login->password);
    json->type = JS_LOG_IN;
    json_str = mx_json_make_json(JS_LOG_IN, json);
    write(fd, json_str, mx_strlen(json_str + 4) + 4);
}

void mx_on_butLogin_clicked(GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    t_login login;

    if (button != NULL) {
        gdk_threads_add_idle (mx_input, mwo);
        login.type = strdup("login");
        login.login = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryLogin_l));
        login.password = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_l));
        login_request(&login, mwo->fd);
        printf("login = %s\npassword = %s\n", login.login, login.password);
    }
}
