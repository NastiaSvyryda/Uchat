#ifndef UCHAT_SERVER_H
#define UCHAT_SERVER_H

#include "libmx.h"
#include <json.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <sqlite3.h>
#include <errno.h>

#define MX_JS_TYPE "{\n\"type\": %d, "
#define MX_JS_TOKEN "\"token\": \"%s\"\n}"
#define MX_JS_STATUS "\"status\": %d, "
#define MX_JS_MSSGE_ID "\"message_id\": %d"
#define MX_JS_FST_LST_NAMES "\"first_name\": \"%s\", \"last_name\": \"%s\""

#define MX_REQ_MES_DEL_IN MX_JS_TYPE MX_JS_MSSGE_ID ", \"client1_id\": %d\n}"
#define MX_REQ_MES_EDIT_IN MX_JS_TYPE MX_JS_MSSGE_ID ", \
 \"new_message\": \"%s\", \"client1_id\": %d\n}"
#define MX_REQ_MES_IN MX_JS_TYPE MX_JS_MSSGE_ID ", \"client1_id\": %d, \
 \"client2_id\": %d, \"new_message\": \"%s\"\n}"

#define MX_RESP_LOG_IN MX_JS_TYPE MX_JS_STATUS "\
 \"user_id\": %d, " MX_JS_FST_LST_NAMES ", " MX_JS_TOKEN
#define MX_RESP_LOG_OUT MX_JS_TYPE "\"status\": %d\n}"
#define MX_RESP_REG MX_JS_TYPE MX_JS_STATUS "\
 \"user_id\": %d, " MX_JS_FST_LST_NAMES ", " MX_JS_TOKEN
#define MX_RESP_MES_DEL_OUT MX_JS_TYPE MX_JS_STATUS MX_JS_MSSGE_ID "\n}"
#define MX_RESP_MES_EDIT_OUT MX_JS_TYPE MX_JS_STATUS MX_JS_MSSGE_ID "\n}"
#define MX_RESP_MES_OUT MX_JS_TYPE MX_JS_STATUS MX_JS_MSSGE_ID ", \
 \"delivery_time\": %ld\n}"

#define MX_TOKEN_LEN 256
#define MX_VARCHAR_LEN 256
#define MX_LOGIN_LEN 256
#define MX_FIRST_NAME_LEN 256
#define MX_LAST_NAME_LEN 256
#define MX_LAST_NAME_LEN 256
#define MX_MAX_NAME_LEN 100
// #define MX_MAX_MESSAGE_LEN 65000

typedef enum e_json_types {
    JS_REG,  // JSON Type - register
    JS_LOG_IN,  // JSON Type - log in
    JS_LOG_OUT,  // JSON Type - log out
    JS_MES_DEL_IN,  // JSON Type - delete message
    JS_MES_DEL_OUT,  // JSON Type - delete message
    JS_MES_EDIT_IN,  // JSON Type - edit message
    JS_MES_EDIT_OUT,  // JSON Type - edit message
    JS_MES_IN,  // JSON Type - input message
    JS_MES_OUT,  // JSON Type - output message
    JS_NUM  // JSON types number
}            t_json_types;

typedef struct s_personal_data {
    char login[MX_VARCHAR_LEN];
    char password[MX_VARCHAR_LEN];
    int user_id;
    char first_name[MX_MAX_NAME_LEN];
    char last_name[MX_MAX_NAME_LEN];
    // char token[MX_TOKEN_LEN];
}              t_personal_data;

typedef struct s_message {
    char *text;
    int client1_id;
    int client2_id;
    int channel_id;
    int message_id;
    time_t delivery_time;
    // char token[MX_TOKEN_LEN];
}              t_message;

union u_json_data {
    t_personal_data pers_info;
    t_message message;
};

typedef struct s_json_data {
    int type;
    int status;
    char token[MX_TOKEN_LEN];
    t_personal_data pers_info;
    t_message message;
}              t_json_data;

typedef struct s_clients {
    struct s_clients *next;
    int fd;
    int name_to;
    int name_from;
    int index;
    struct s_clients *first;
}               t_clients;

int main(int argc, char **argv);
void mx_routes(t_json_data *json, t_clients *client, t_clients *cur_client);
///Config
char *mx_config_sqlite3_db_name(void);
///end config

///Validation
void mx_valid_check_argc_error(int argc);
void mx_valid_sqlite3_open_db(int rc, sqlite3 *db);
void mx_valid_sqlite3_failed_data(int rc, sqlite3 *db, char *err_msg);
int mx_valid_str_isalpha(char *str);
///end validation

///JSON
//Basic
void mx_str_to_file(const char *filepath, const char *data);
t_json_data *mx_json_parse(char *s);
char *mx_json_make_json(enum e_json_types type, t_json_data *data);
//Auth
char *mx_json_log_in_response(t_json_data *data);
char *mx_json_log_out_response(t_json_data *data);
char *mx_json_register_response(t_json_data *data);
//Message
char *mx_json_message_in_request(t_json_data *data);
char *mx_json_message_out_response(t_json_data *data);
char *mx_json_message_edit_in_request(t_json_data *data);
char *mx_json_message_edit_out_response(t_json_data *data);
char *mx_json_message_delete_in_request(t_json_data *data);
char *mx_json_message_delete_out_response(t_json_data *data);
///end JSON

///Controllers
//Auth
void mx_controller_login(t_json_data *json, t_clients *client);
void mx_controller_register(t_json_data *json, t_clients *client);
void mx_controller_log_out(void);
//Messages
void mx_controller_message(t_clients *client, t_clients *cur_client, t_json_data *json);
///end controllers

///Models
//model_channel
char *mx_model_channel_database(void);
char *mx_model_channel_name_table(void);
char **mx_model_channel_fill_table(void);
//model_delivery_user
char *mx_model_delivery_user_database(void);
char *mx_model_delivery_user_name_table(void);
char **mx_model_delivery_user_fill_table(void);
//model_message
char *mx_model_message_database(void);
char *mx_model_message_name_table(void);
char **mx_model_message_fill_table(void);
//model_user
char *mx_model_user_database(void);
char *mx_model_user_channel_name_table(void);
char **mx_model_user_fill_table(void);
//model_user_channel
char *mx_model_user_channel_database(void);
char *mx_model_user_name_table(void);
char **mx_model_user_channel_fill_table(void);
///end models

///Migrations
void mx_migration(char *argv);
void mx_migration_users(bool status);
void mx_migration_channels(bool status);
void mx_migration_messages(bool status);
void mx_migration_user_channel(bool status);
void mx_migration_delivery_user(bool status);
///end migrations

///System
//Basic
void mx_conn_list_sock(int *fd, char **argv);
struct sockaddr_in mx_accept_connections(t_clients *client, int listenfd);
void mx_thread_create(t_clients *client, struct sockaddr_in cli);
//CRUD
char *mx_create_token(int length);
void mx_create_databases(char *database, char *table, char *fill_table, char *value_table);
t_list *mx_read_database(char *database, char *table, char *fill_table, char *where);
void mx_update_database(char *database, char *table, char *set, char *where);
void mx_delete_database(char *database, char *table, char *fill_table, char *where);
//end systeam

#endif
