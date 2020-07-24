#include "uchat_client.h"

void mx_on_logout_clicked(__attribute__((unused)) GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_LOG_OUT;
    json->user_id = mwo->user_id;
    strcpy(json->token, mwo->token);

    json_str = mx_json_make_json(JS_LOG_OUT, json);

    if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
    mx_set_component(mwo, mwo->loginWindow);
}

void mx_update_user_info(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    const gchar *login = gtk_entry_get_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "login"));
    const gchar *first_name = gtk_entry_get_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "first_name"));
    const gchar *last_name = gtk_entry_get_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "last_name"));

    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_PERS_INFO_UPD;
    json->user_id = mwo->user_id;
    strcpy(json->token, mwo->token);

    strcpy(json->pers_info.login, login);
    strcpy(json->pers_info.first_name, first_name);
    strcpy(json->pers_info.last_name, last_name);

    json_str = mx_json_make_json(JS_PERS_INFO_UPD, json);

    if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, mx_strlen(json_str + 4) + 4);
}

void mx_on_info_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{ //создавать новое диалоговое окно
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mwo->infoDialog = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "dialog_info"));
    gtk_entry_set_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "login"), mwo->login);
    gtk_entry_set_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "first_name"), mwo->first_name);
    gtk_entry_set_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "last_name"), mwo->last_name);

    gtk_widget_show_all(mwo->infoDialog);
    gtk_main();
}

void mx_exit_clicked_info(__attribute__((unused)) GtkWidget *button, __attribute__((unused)) gpointer data)
{
    //t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;

    //    gtk_widget_hide(mwo->infoDialog);
    //    gtk_main_quit();
    //    gtk_widget_destroy (mwo->infoDialog);
}
