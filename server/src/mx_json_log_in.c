#include "../../inc/header.h"

char *mx_json_log_in_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_LOG_IN, JS_LOG_IN, data->pers_info.login,
             data->pers_info.password, data->token);
    *(int *)s = len;

    return s;
}

char *mx_json_log_in_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_RESP_LOG_IN, JS_LOG_IN, data->status, data->user_id,
             data->pers_info.first_name, data->pers_info.last_name,
             data->token);
    *(int *)s = len;

    return s;
}
