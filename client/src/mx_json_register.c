#include "../../inc/header.h"

char *mx_json_register_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_REG, JS_REG, data->pers_info.login, data->pers_info.password,
             data->pers_info.first_name, data->pers_info.last_name);
    *(int *)s = len;

    return s;
}

char *mx_json_register_response(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_RESP_REG, JS_REG, data->status, data->user_id,
             data->pers_info.first_name, data->pers_info.last_name, data->token);
    *(int *)s = len;

    return s;
}
