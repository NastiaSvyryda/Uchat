#include "../inc/header.h"

char *mx_json_register_request(t_personal_data *data) {
    char *s;

    asprintf(&s, MX_REQ_REG, "register", data->login, data->password,
             data->first_name, data->last_name);
    return s;
}
char *mx_json_register_response(t_personal_data *data, int status) {
    char *s;

    asprintf(&s, MX_RESP_REG, "register", status, data->token, data->user_id,
             data->first_name, data->last_name);
    return s;
}
