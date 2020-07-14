#ifndef UCHAT_UCHAT_H
#define UCHAT_UCHAT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <json.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include "libmx.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

//GTK

#define UI_FILE "login.glade"
#define MX_JSON_TEMPLATE "./client/resources/client_type_%d.json"

#define MX_TOKEN_LEN 256
#define MX_VARCHAR_LEN 256
#define MX_LOGIN_LEN 256
#define MX_FIRST_NAME_LEN 256
#define MX_LAST_NAME_LEN 256
#define MX_LAST_NAME_LEN 256
#define MX_MAX_NAME_LEN 100
// #define MX_MAX_MESSAGE_LEN 65000

typedef struct s_login {
    char *type;
    char *login;
    char *password;
}               t_login;

typedef struct s_registre {
    char *type;
    char *login;
    char *password;
    char *name;
    char *surname;
}               t_registre;

typedef struct s_MainWindowObjects {//changed
    GtkWindow *loginWindow;
    GtkWindow *registreWindow;
    GtkWindow *mainWindow;
    GtkWindow *chatWindow;

    GtkEntry  *entryLogin_l;
    GtkEntry  *entryPass_l;

    GtkEntry  *entryLogin_r;
    GtkEntry  *entryPass_r;
    GtkEntry  *entryPass_r2;
    GtkEntry  *entryName_r;
    GtkEntry  *entrySurname_r;
    GtkEntry  *entryMessage;

    GtkWidget *row;
    GtkWidget *mess_row;
    GtkWidget *list;
    GtkWidget *messageList;
    int fd;
    int user_id;
    char token[MX_TOKEN_LEN + 1];
    SSL *ssl;
} t_mainWindowObjects;

///TLS
int mx_open_connection(const char *hostname, int port);
SSL_CTX* mx_init_ctx(void);
void mx_show_certs(SSL* ssl);
///end TLS

//Buttons
gboolean mx_reciever(__attribute__((unused)) GIOChannel *chan, __attribute__((unused)) GIOCondition condition, void *data);
void mx_on_butLogin_clicked(GtkWidget *window, gpointer data);
void mx_on_butRegistreIn_clicked(GtkWidget *button, gpointer data);
void mx_on_butRegistre_clicked(GtkWidget *window, gpointer data);

GtkWidget *mx_create_chat(const gchar *text, struct s_MainWindowObjects *mwo);
GtkWidget *mx_create_message(const gchar *text, struct s_MainWindowObjects *mwo);
void mx_on_chat_clicked(GtkWidget *button, gpointer data);
void mx_create_main_window(struct s_MainWindowObjects *mwo);
void mx_create_login_window(t_mainWindowObjects main, int fg);
void mx_create_registre_window(struct s_MainWindowObjects *mwo);

//gtk

typedef enum e_json_types {
    JS_REG,          // JSON Type - register
    JS_LOG_IN,       // JSON Type - log in
    JS_LOG_OUT,      // JSON Type - log out
    JS_MES_DEL_IN,   // JSON Type - delete message
    JS_MES_DEL_OUT,  // JSON Type - delete message
    JS_MES_EDIT_IN,  // JSON Type - edit message
    JS_MES_EDIT_OUT, // JSON Type - edit message
    JS_MES_IN,       // JSON Type - input message
    JS_MES_OUT,      // JSON Type - output message
    JS_NUM           // JSON types number
} t_json_types;



typedef struct s_personal_data
{
    char login[MX_VARCHAR_LEN];
    char password[MX_VARCHAR_LEN];
    int user_id;
    char first_name[MX_MAX_NAME_LEN];
    char last_name[MX_MAX_NAME_LEN];
    // char token[MX_TOKEN_LEN];
} t_personal_data;

typedef struct s_message
{
    char *text;
    int client1_id;
    int channel_id;
    int message_id;
    time_t delivery_time;
    // char token[MX_TOKEN_LEN];
} t_message;

// union u_json_data {
//     t_personal_data pers_info;
//     t_message message;
// };

typedef struct s_json_data
{
    int type;
    int status;
    char token[MX_TOKEN_LEN + 1];
    t_personal_data pers_info;
    t_message message;
} t_json_data;

void mx_str_to_file(const char *filepath, const char *data);

char *mx_json_log_in_request(t_json_data *data);
char *mx_json_log_out_request(t_json_data *data);
char *mx_json_message_delete_in_response(t_json_data *data);
char *mx_json_message_delete_out_request(t_json_data *data);
char *mx_json_message_edit_in_response(t_json_data *data);
char *mx_json_message_edit_out_request(t_json_data *data);
char *mx_json_message_in_response(t_json_data *data);
char *mx_json_message_out_request(t_json_data *data);
char *mx_json_register_request(t_json_data *data);
char *mx_get_json_format_template(enum e_json_types type);
char *mx_json_make_json(enum e_json_types type, t_json_data *data);
t_json_data *mx_json_parse(char *s);

#endif //UCHAT_UCHAT_H
