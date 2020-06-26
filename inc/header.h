#pragma once

#include "../libmx/inc/libmx.h"

typedef enum e_json_types {
    JS_MES_IN,  // JSON Type - input message
    JS_MES_OUT,  // JSON Type - output message
    JS_MES_EDIT_IN,  // JSON Type - edit message
    JS_MES_EDIT_OUT,  // JSON Type - edit message
    JS_MES_DEL_IN,  // JSON Type - delete message
    JS_MES_DEL_OUT,  // JSON Type - delete message
    JS_REG,  // JSON Type - register
    JS_LOG_IN,  // JSON Type - log in
    JS_LOG_OUT  // JSON Type - log out
} t_json_types;


#define MX_REQ_LOG_IN "{ \"type\": \"%s\", \"login\": \"%s\", \
    \"password\": \"%s\", \"token\": \"%s\" } "
#define MX_REQ_LOG_OUT "{ \"type\": \"%s\", \"user_id\": %d, \
    \"token\": \"%s\" }"
#define MX_REQ_MES_DEL "{ \"type\": \"%s\", \"message_id\": %d, \
    \"client1_id\": %d }"
#define MX_REQ_MES_DEL_OUT "{ \"type\": \"%s\", \"message_id\": %d, \
    \"client1_id\": %d, \"token\": \"%s\" }"
#define MX_REQ_MES_EDIT_IN "{ \"type\": \"%s\", \"message_id\": %d, \
    \"new_message\": \"%s\", \"client1_id\": %d }"
#define MX_REQ_MES_EDIT_OUT "{ \"type\": \"%s\", \"message_id\": %d, \
    \"new_message\": \"%s\", \"client1_id\": %d, \"token\": \"%s\" }"
#define MX_REQ_MES_IN "{ \"type\": \"%s\", \"from_id\": %d, \
    \"to_id\": %d, \"message\": \"%s\", \"message_id\": %d }"
#define MX_REQ_MES_OUT "{ \"type\": \"%s\", \"from_id\": %d, \
    \"to_id\": %d, \"message\": \"%s\" }"
#define MX_REQ_REG "{ \"type\": \"%s\", \"login\": \"%s\", \
    \"password\": \"%s\", \"name\": \"%s\", \"last_name\": \"%s\" }"

#define MX_RESP_LOG_IN "{ \"type\": \"%s\", \"status\": %d, \"token\": %d, \
    \"user_id\": %d, \"name\": \"%s\", \"last_name\": \"%s\" }"
#define MX_RESP_LOG_OUT "{ \"type\": \"%s\", \"status\": %d }"
#define MX_RESP_MES_DEL "{ \"type\": \"%s\", \"status\": %d, \
    \"message_id\": %d, \"client2_id\": %d, \"token\": %d }"
#define MX_RESP_MES_DEL_OUT "{ \"type\": \"%s\", \"status\": %d, \
    \"message_id\": %d }"
#define MX_RESP_MES_EDIT_IN "{ \"type\": \"%s\", \"status\": %d, \
    \"message_id\": %d, \"client2_id\": %d, \"token\": %d }"
#define MX_RESP_MES_EDIT_OUT "{ \"type\": \"%s\", \"status\": %d, \
    \"message_id\": %d }"
#define MX_RESP_MES_IN "{ \"type\": \"%s\", \"status\": %d, \
    \"message_id\": %d }"
#define MX_RESP_MES_OUT "{ \"type\": \"%s\", \"status\": %d, \
    \"deliver_time\": %d, \"message_id\": %d }"
#define MX_RESP_REG "{ \"type\": \"%s\", \"status\": %d, \"token\": %d, \
    \"user_id\": \"%s\", \"name\": \"%s\", \"last_name\": \"%s\" }"

#define MX_VARCHAR_LEN 256
#define MX_LOGIN_LEN 256
#define MX_FIRST_NAME_LEN 256
#define MX_LAST_NAME_LEN 256
#define MX_LAST_NAME_LEN 256





typedef struct s_personal_data {
    char login[MX_VARCHAR_LEN];
    char password[MX_VARCHAR_LEN];
    char token[MX_VARCHAR_LEN];
    int user_id;
    char first_name[MX_VARCHAR_LEN];
    char last_name[MX_VARCHAR_LEN];
}              t_personal_data;
































