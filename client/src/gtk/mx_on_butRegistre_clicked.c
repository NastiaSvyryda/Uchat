#include "uchat_client.h"

gboolean mx_input(gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    char *json_str = NULL;
    int length = 0;
    t_json_data *json = NULL;

    while (1) {
        read(mwo->fd, &length, 4);
        json_str = mx_strnew(length);
        read(mwo->fd, json_str, length);
        json = mx_json_parse(json_str);
        mx_printstr("Response recieved: \n");
        if (json->type == JS_REG) {
            if (json->status == 200) {
                gtk_window_close(mwo->registreWindow);
                mx_create_main_window(mwo);
                break;
            }
            if (json->status == 412)
                mx_printstr("fail fills");
        }
        else if (json->type == JS_LOG_IN) {
            if (json->status == 200) {
                gtk_window_close(mwo->loginWindow);
                mx_create_main_window(mwo);
                break;
            }
        }
    }
    return FALSE;
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
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    t_registre registre;

    if (button != NULL) {
        gdk_threads_add_idle (mx_input, mwo);
        puts("registre_button");
        registre.type = strdup("register");
        registre.login = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryLogin_r));
        if (strcmp((char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r)), (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r2))) == 0)
            registre.password = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r));
        else
            printf("different passwords\n");
        registre.name = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryName_r));
        registre.surname = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entrySurname_r));
        registre_request(&registre, mwo->fd);
        printf("login = %s\npassword = %s\nname = %s\nsurname = %s\n",
               registre.login, registre.password, registre.name,
               registre.surname);
    }
}

void mx_on_butRegistreIn_clicked(GtkWidget *button, gpointer data) {
    if (button != NULL) {
        t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
        gtk_window_close(mwo->loginWindow);
        mx_create_registre_window(mwo);
    }
}
