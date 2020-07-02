#include "uchat_server.h"

char *mx_json_register_response(t_personal_data *data, int status, char *token) {
    char *s;

    asprintf(&s, MX_RESP_REG, JS_REG, status, data->user_id,
             data->first_name, data->last_name, token);
    return s;
}
