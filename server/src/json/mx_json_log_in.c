#include "uchat_server.h"

char *mx_json_log_in_response(t_personal_data *data, int status, char *token) {
    char *s;

    asprintf(&s, MX_RESP_LOG_IN, JS_LOG_IN, status, data->user_id,
             data->first_name, data->last_name, token);
    return s;
}
