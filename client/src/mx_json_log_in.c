#include "../inc/client.h"

char *mx_json_log_in_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_LOG_IN, JS_LOG_IN, data->pers_info.login,
             data->pers_info.password, data->token);
    *(int *)s = len;

    return s;
}