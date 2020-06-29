#include "../inc/header.h"

char *mx_json_log_in_request(t_personal_data *data, char *token) {
    char *s;

    asprintf(&s, MX_REQ_LOG_IN, JS_LOG_IN, data->login, data->password,
             token);
    return s;
}

char *mx_json_log_in_response(t_personal_data *data, int status, char *token) {
    char *s;

    asprintf(&s, MX_RESP_LOG_IN, JS_LOG_IN, status, data->user_id,
             data->first_name, data->last_name, token);
    return s;
}
