#include "../inc/header.h"

char *mx_json_log_in_request(t_personal_data *data) {
    char *s;

    asprintf(&s, MX_REQ_LOG_IN, "log_in", data->login, data->password,
             data->token);
    return s;
}

char *mx_json_log_in_response(t_personal_data *data, int status) {
    char *s;

    asprintf(&s, MX_RESP_LOG_IN, "log_in", status, data->token, data->user_id,
             data->first_name, data->last_name);
    return s;
}
