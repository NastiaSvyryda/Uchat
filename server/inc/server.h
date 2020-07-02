#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <err.h>
#include <signal.h>
#include <stdarg.h>
#include <ctype.h>
#include <json.h>
#include "../../libmx/inc/libmx.h"

#define MX_JS_TYPE "{ \"type\": %d, "
#define MX_JS_TOKEN "\"token\": \"%s\" }"
#define MX_JS_STATUS "\"status\": %d, "
#define MX_JS_MSSGE_ID "\"message_id\": %d"
#define MX_JS_FST_LST_NAMES "\"first_name\": \"%s\", \"last_name\": \"%s\""

#define MX_REQ_MES_DEL_IN MX_JS_TYPE MX_JS_MSSGE_ID ", \"client1_id\": %d }"
#define MX_REQ_MES_EDIT_IN MX_JS_TYPE MX_JS_MSSGE_ID ", \
    \"new_message\": \"%s\", \"client1_id\": %d }"
#define MX_REQ_MES_IN MX_JS_TYPE MX_JS_MSSGE_ID ", \"client1_id\": %d, \
    \"client2_id\": %d, \"new_message\": \"%s\" }"

#define MX_RESP_LOG_IN MX_JS_TYPE MX_JS_STATUS "\
    \"user_id\": %d, " MX_JS_FST_LST_NAMES ", " MX_JS_TOKEN
#define MX_RESP_LOG_OUT MX_JS_TYPE "\"status\": %d }"
#define MX_RESP_REG MX_JS_TYPE MX_JS_STATUS "\
        \"user_id\": %d, " MX_JS_FST_LST_NAMES ", " MX_JS_TOKEN
#define MX_RESP_MES_DEL_OUT MX_JS_TYPE MX_JS_STATUS MX_JS_MSSGE_ID \
        " }"
#define MX_RESP_MES_EDIT_OUT MX_JS_TYPE MX_JS_STATUS MX_JS_MSSGE_ID " }"
#define MX_RESP_MES_OUT MX_JS_TYPE MX_JS_STATUS MX_JS_MSSGE_ID ", \
        \"delivery_time\": %ld }"

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
    int user_id;
}              t_json_data;

void mx_str_to_file(const char *filepath, const char *data);

char *mx_json_log_in_response(t_json_data *data);
char *mx_json_log_out_response(t_json_data *data);
char *mx_json_message_delete_in_request(t_json_data *data);
char *mx_json_message_delete_out_response(t_json_data *data);
char *mx_json_message_edit_in_request(t_json_data *data);
char *mx_json_message_edit_out_response(t_json_data *data);
char *mx_json_message_in_request(t_json_data *data);
char *mx_json_message_out_response(t_json_data *data);
char *mx_json_register_response(t_json_data *data);
char *mx_json_make_json(enum e_json_types type, t_json_data *data);
t_json_data *mx_json_parse(char *s);
























