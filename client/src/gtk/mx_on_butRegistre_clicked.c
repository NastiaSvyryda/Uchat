#include "uchat_client.h"


void mx_add_out_message(t_mainWindowObjects *mwo, char *text_char) {
    GtkWidget *row;
    gchar *text;
    if (text_char == NULL)
        puts("NULL\n");
    text = g_strdup_printf(text_char);
    row = mx_create_row(text, mwo);//change signal connectors
    gtk_list_box_insert(GTK_LIST_BOX(mwo->messageList), row, -1);
    gtk_widget_show_all(GTK_WIDGET(mwo->chatWindow));
}
gboolean mx_input(__attribute__((unused)) GIOChannel *chan, __attribute__((unused)) GIOCondition condition, void *data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *json_str = NULL;
    int length = 0;
    t_json_data *json = NULL;
        read(mwo->fd, &length, 4);
        json_str = mx_strnew(length);
        read(mwo->fd, json_str, length);
        json = mx_json_parse(json_str);
        mx_printstr("Response recieved:\n");
        mx_printstr(json_str + 4);
        mx_printchar('\n');
        if (json->type == JS_REG) {
            if (json->status == 200) {
                gtk_window_close(mwo->registreWindow);
                mx_create_main_window(mwo);
            }
        }
        else if (json->type == JS_LOG_IN) {
            if (json->status == 200) {
                gtk_window_close(mwo->loginWindow);
                mx_create_main_window(mwo);
            }
        }
        else if (json->type == JS_MES_IN) {
            puts(json->message.text);
            mx_add_out_message(mwo, (gchar *) json->message.text);
        }
    return TRUE;
}

static void registre_request(t_registre *registre, int fd) {
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    json->type = JS_REG;
    strcpy(json->pers_info.login, registre->login);
    strcpy(json->pers_info.password, registre->password);
    strcpy(json->pers_info.first_name, registre->name);
    strcpy(json->pers_info.last_name, registre->surname);
    json_str = mx_json_make_json(JS_REG, json);
    write(fd, json_str, mx_strlen(json_str + 4) + 4);
}

void mx_on_butRegistre_clicked(GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    t_registre registre;

    if (button != NULL) {
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
        registre_request(&registre, mwo->fd);
        printf("login = %s\npassword = %s\nname = %s\nsurname = %s\n",
               registre.login, registre.password, registre.name,
               registre.surname);
    }
}

void mx_on_butRegistreIn_clicked(GtkWidget *button, gpointer data) {
    if (button != NULL) {
        t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
        gtk_window_close(mwo->loginWindow);
        mx_create_registre_window(mwo);
    }
}
