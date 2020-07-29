#include "uchat_client.h"

static void update_user_request(t_mainWindowObjects *mwo, char *first_name,
                                    char *last_name, char *new_pass) {
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_PERS_INFO_UPD;
    json->user_id = mwo->user_id;
    strcpy(json->token, mwo->token);
    strcpy(json->pers_info.login, mwo->login);
    if (strlen((char *) first_name) != 0) {
        strcpy(json->pers_info.first_name, first_name);
        memset(mwo->first_name, '\0', MX_MAX_NAME_LEN + 1);
        strcpy(mwo->first_name, first_name);
    }
    if (strlen((char *) last_name) != 0) {
        strcpy(json->pers_info.last_name, last_name);
        memset(mwo->last_name, '\0', MX_MAX_NAME_LEN + 1);
        strcpy(mwo->last_name, last_name);
    }
    if (strlen((char *) new_pass) != 0)
        strcpy(json->pers_info.password, new_pass);
    else
        strcpy(json->pers_info.password, "");
    json_str = mx_json_make_json(JS_PERS_INFO_UPD, json);
    if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, *(int *) json_str + 4);
}

void mx_update_user_info(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *first_name = mx_handle_user_input((char *)gtk_entry_get_text(mwo->edit_name));
    char *last_name = mx_handle_user_input((char *)gtk_entry_get_text(mwo->edit_surname));
    char *new_pass = mx_handle_user_input((char *)gtk_entry_get_text(mwo->new_pass));
    char *new_pass_r = mx_handle_user_input((char *)gtk_entry_get_text(mwo->new_pass_r));
    _Bool valid;

    if (strcmp(new_pass, new_pass_r) != 0) {
        mx_show_popup(mwo->Window, "Different passwords!");
        return;
    }
    valid = (strlen((char *)new_pass) == 0 || mx_validate_password((gchar *)new_pass, mwo->Window)) &&
            (strlen((char *)first_name) == 0 || mx_validate_user_name((gchar *)first_name, mwo->Window)) &&
            (strlen((char *)last_name) == 0 || mx_validate_user_surname((gchar *)last_name, mwo->Window)) &&
            (strlen((char *)new_pass) > 0 || strlen((char *)first_name) > 0 || strlen((char *)last_name) > 0);
    if (valid) {
        update_user_request(mwo, first_name, last_name, new_pass);
        mwo->info = 0;
    }
}

void mx_on_info_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    GtkBuilder *builder;
    GError *error = NULL;

    if (mwo->info == 0) {
        mwo->info = 1;
        builder = gtk_builder_new();
        if (!gtk_builder_add_from_file(builder, UI_FILE, &error)) {
            mx_printstr(error->message);
        }
        mwo->infoDialog = GTK_WIDGET(
                 gtk_builder_get_object(builder, "dialog_info"));
        gtk_window_set_resizable (GTK_WINDOW (mwo->infoDialog), FALSE);
        mwo->edit_name = (GtkEntry *)gtk_builder_get_object(builder, "first_name");
        mwo->edit_surname = (GtkEntry *)gtk_builder_get_object(builder, "last_name");
        mwo->new_pass = (GtkEntry *)gtk_builder_get_object(builder, "update_password");
        mwo->new_pass_r = (GtkEntry *)gtk_builder_get_object(builder, "update_password1");
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "label_login_profile")),(const gchar *)mwo->login);
        gtk_entry_set_text(mwo->edit_name, mwo->first_name);
        gtk_entry_set_text(mwo->edit_surname, mwo->last_name);
        gtk_builder_connect_signals(builder, mwo);
        gtk_window_set_transient_for(GTK_WINDOW(mwo->infoDialog),
                                      mwo->Window);
        gtk_window_set_position(GTK_WINDOW(mwo->infoDialog),
                                 GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_widget_show(mwo->infoDialog);
    }
}

void mx_exit_clicked_info(__attribute__((unused)) GtkWidget *button, __attribute__((unused)) gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mwo->info = 0;
    gtk_widget_destroy(mwo->infoDialog);
}
