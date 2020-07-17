#ifndef UCHAT_UCHAT_H
#define UCHAT_UCHAT_H
#define _GNU_SOURCE

#include <ctype.h>
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
#define MX_JSON_TO_STR_FLAGS JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY

#define MX_TOKEN_LEN 256
#define MX_VARCHAR_LEN 256
#define MX_LOGIN_LEN 256
#define MX_FIRST_NAME_LEN 256
#define MX_LAST_NAME_LEN 256
#define MX_LAST_NAME_LEN 256
#define MX_MAX_NAME_LEN 100
// #define MX_MAX_MESSAGE_LEN 65000



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
    JS_GET_USERS,    // JSON Type - get all the users logins and ids
    JS_MES_HIST,     // JSON Type - load channel messages history
    JS_NUM           // JSON types number
}            t_json_types;




typedef struct s_personal_data {
    char login[MX_VARCHAR_LEN + 1];
    char password[MX_VARCHAR_LEN + 1];
    char first_name[MX_MAX_NAME_LEN + 1];
    char last_name[MX_MAX_NAME_LEN + 1];
}              t_personal_data;

typedef struct s_message {
    int client1_id;
    int message_id;
    time_t delivery_time;
    char *text;
    int channel_id;
    int last_message_id;
}              t_message;

typedef struct s_channel {
    int channel_id;
    char channel_name[MX_VARCHAR_LEN + 1];
    int *user_ids;
    int user_ids_size;
    time_t last_mes_time;
}              t_channel;

typedef struct s_id_login {
    int user_id;
    char login[MX_VARCHAR_LEN + 1];
}              t_id_login;

typedef struct s_json_data {
    int type;
    int status;
    int user_id;
    char token[MX_TOKEN_LEN + 1];
    t_personal_data pers_info;
    t_message message;
    bool new_channel;
    t_channel new_channel_data;
    t_channel *channels_arr;
    int channels_arr_size;
    t_id_login *ids_logins_arr;
    int ids_logins_arr_size;
    t_message *messages_arr;
    int messages_arr_size;
}              t_json_data;
typedef struct s_message_list {
    int message_id;
    time_t delivery_time;
//    char *text;
    int channel_id;
    int last_message_id;
    struct s_message_list *first;
    struct s_message_list *next;
}              t_message_list;

typedef struct s_channel_info {
    t_channel channel_data;
    GtkWidget *chat_button;
    t_message_list *message;
    struct s_channel_info *first;
    struct s_channel_info *next;
}               t_channel_info;


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
    GtkBuilder *builder;

    GtkWindow *Window;

    GtkWidget *loginWindow;
    GtkWidget *registreWindow;
    GtkWidget *mainWindow;
    GtkWidget *chatWindow;

    GtkWidget *infoDialog;
    GtkWidget *addChat_Dialog;

    GtkEntry  *entryLogin_l;
    GtkEntry  *entryPass_l;

    GtkEntry  *entryLogin_r;
    GtkEntry  *entryPass_r;
    GtkEntry  *entryPass_r2;
    GtkEntry  *entryName_r;
    GtkEntry  *entrySurname_r;

    GtkWidget  *entryMessage;

    GtkWidget *row;
    GtkWidget *mess_row;
    GtkWidget *list;
    GtkWidget *messageList;

    int fd;
    int user_id;
    char login[MX_VARCHAR_LEN + 1];
    char first_name[MX_MAX_NAME_LEN + 1];
    char last_name[MX_MAX_NAME_LEN + 1];
    char token[MX_TOKEN_LEN + 1];
    SSL *ssl;
    t_channel_info *channel_info;
} t_mainWindowObjects;


///TLS
int mx_open_connection(const char *hostname, int port);
SSL_CTX* mx_init_ctx(void);
void mx_show_certs(SSL* ssl);
///end TLS

//Buttons
gboolean mx_reciever(__attribute__((unused)) GIOChannel *chan, __attribute__((unused)) GIOCondition condition, void *data);
void mx_onExit_clicked(__attribute__((unused)) GtkWidget *button, __attribute__((unused)) gpointer data);
void mx_on_butLogin_clicked(__attribute__((unused))GtkWidget *button, gpointer data);
void mx_on_butRegistreIn_clicked(__attribute__((unused))GtkWidget *button, gpointer data);
void mx_on_butRegistre_clicked(__attribute__((unused))GtkWidget *button, gpointer data);
void mx_info_clicked(__attribute__((unused))GtkWidget *button, gpointer data);

GtkWidget *mx_create_chat(const gchar *text, struct s_MainWindowObjects *mwo);
GtkWidget *mx_create_message(const gchar *text, struct s_MainWindowObjects *mwo, int align);
void mx_on_chat_clicked(GtkWidget *button, gpointer data);
void mx_create_login_window(t_mainWindowObjects *main);
void mx_create_registre_window(struct s_MainWindowObjects *mwo);

void mx_set_component(t_mainWindowObjects *mwo, GtkWidget *gtk_component);
_Bool mx_valid_string(char *str);
void mx_show_popup(void *parent_window, char *msg);
//gtk


void mx_str_to_file(const char *filepath, const char *data);

char *mx_json_register_request(t_json_data *data);
char *mx_json_log_in_request(t_json_data *data);
char *mx_json_log_out_request(t_json_data *data);
char *mx_json_message_delete_in_response(t_json_data *data);
char *mx_json_message_delete_out_request(t_json_data *data);
char *mx_json_message_edit_in_response(t_json_data *data);
char *mx_json_message_edit_out_request(t_json_data *data);
char *mx_json_message_in_response(t_json_data *data);
char *mx_json_message_out_request(t_json_data *data);
char *mx_json_get_users_request(t_json_data *data);
char *mx_json_message_history_request(t_json_data *data);
char *mx_get_json_format_template(enum e_json_types type);
char *mx_json_make_json(enum e_json_types type, t_json_data *data);
void mx_parse_channels(struct json_object *ch_arr, t_json_data *data);
int mx_parse_new_channel(struct json_object *jo, t_json_data *json);
void mx_parse_logins(struct json_object *u_logins_arr, t_json_data *data);
void mx_parse_messages(struct json_object *messages_arr, t_json_data *data);
t_json_data *mx_json_parse(char *s);

#endif //UCHAT_UCHAT_H
