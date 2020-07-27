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
        SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
    mwo->info = 0;
    gtk_widget_destroy(mwo->infoDialog);
    mx_set_component(mwo, mwo->loginWindow);
}

void mx_update_user_info(__attribute__((unused)) GtkWidget *button, gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

//    const gchar *login = gtk_entry_get_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "login"));
//    const gchar *first_name = gtk_entry_get_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "first_name"));
//    const gchar *last_name = gtk_entry_get_text((GtkEntry *)gtk_builder_get_object(mwo->builder, "last_name"));

    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_PERS_INFO_UPD;
    json->user_id = mwo->user_id;
    strcpy(json->token, mwo->token);

    strcpy(json->pers_info.login, "spleshkov");
    strcpy(json->pers_info.first_name, "Viacheslav");
    strcpy(json->pers_info.last_name, "Pleshkov");
    strcpy(json->pers_info.password, "rootroot");

    json_str = mx_json_make_json(JS_PERS_INFO_UPD, json);

    if (SSL_connect(mwo->ssl) == -1) /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
        SSL_write(mwo->ssl, json_str, *(int *)json_str + 4);
    mwo->info = 0;
    gtk_widget_destroy(mwo->infoDialog);
}

void mx_on_info_clicked(__attribute__((unused)) GtkWidget *button, gpointer data)
{ //создавать новое диалоговое окно
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    GtkBuilder *builder;
    GError *error = NULL;
    if (mwo->info == 0) {
        mwo->info = 1;
        builder = gtk_builder_new();
    //   gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
    //                                               GTK_STYLE_PROVIDER(mainObjects->provider),
    //                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
        if (!gtk_builder_add_from_file(builder, "sent_gui.glade", &error)) {
            mx_printstr(error->message);
        }
        mwo->infoDialog = GTK_WIDGET(
                 gtk_builder_get_object(builder, "dialog_info"));
        gtk_entry_set_text((GtkEntry *)gtk_builder_get_object(builder, "login"), mwo->login);
        gtk_entry_set_text((GtkEntry *)gtk_builder_get_object(builder, "first_name"), mwo->first_name);
        gtk_entry_set_text((GtkEntry *)gtk_builder_get_object(builder, "last_name"), mwo->last_name);
        gtk_builder_connect_signals(builder, mwo);
        gtk_window_set_transient_for(GTK_WINDOW(mwo->infoDialog),
                                      mwo->Window);
        gtk_window_set_position(GTK_WINDOW(mwo->infoDialog),
                                 GTK_WIN_POS_CENTER_ON_PARENT);
        gtk_widget_show(mwo->infoDialog);
    }
}

void mx_exit_clicked_info(__attribute__((unused)) GtkWidget *button, __attribute__((unused)) gpointer data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    mwo->info = 0;
    gtk_widget_destroy(mwo->infoDialog);
}
