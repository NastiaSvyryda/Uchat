#include "uchat_client.h"

char *mx_json_register_request(t_json_data *data) {
    char *s;
    int len = asprintf(&s, "0000" MX_REQ_REG, JS_REG, data->pers_info.login, data->pers_info.password,
            data->pers_info.first_name, data->pers_info.last_name);
    *(int *)s = len;

    return s;
}
